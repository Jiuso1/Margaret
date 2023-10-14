#include "dialogo_anadirprograma.h"
#include "motor.h"
#include "windowinfo.h"

#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <string>
#include <windows.h>
#include <QString>
#include <QFile>

//Constructor.
dialogo_anadirprograma::dialogo_anadirprograma(QWidget *parent) : QDialog(parent)
{
    //Inicializamos las variables:
    aceptarButton = new QPushButton(tr("&Aceptar"));//aceptarButton tiene el texto Aceptar y atajo de teclado Alt + A.
    cancelarButton = new QPushButton(tr("&Cancelar"));//cancelarButton tiene el texto Cancelar y atajo de teclado Alt + C.
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    nMaxProgramas = 50;//El nº máximo de programas que Margaret puede monitorizar es 50.

    //Inicializamos los punteros a nullptr:
    for(int i = 0;i < nMaxProgramas;i++){
        check[i] = nullptr;
    }

    actualizarProgramasAbiertos();//Llena el boxLayout de QCheckbox con el nombre de programas abiertos actualmente.

    programasBox->setTitle(tr("Seleccione los programas que desee monitorizar"));//Asigna el texto que aparece por encima de los QCheckbox.
    programasBox->setLayout(boxLayout);//Vincula el boxLayout lleno de programas con programasBox.

    //Añadimos los widgets a los layout:

    //Como cancelarButton y aceptarButton se disponen horizontalmente, se añaden a horizontalLayout:
    horizontalLayout->addWidget(cancelarButton);
    horizontalLayout->addWidget(aceptarButton);

    //Como programasBox y horizontalLayout se disponen verticalmente, se añaden a verticalLayout:
    verticalLayout->addWidget(programasBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);//El diálogo tiene de layout verticalLayout.

    setAttribute(Qt::WA_DeleteOnClose);//Se activa la flag WA_DeleteOnClose. Cuando se cierre el diálogo se dejará de reservar toda su memoria.

    connect(aceptarButton,SIGNAL(clicked()),this,SLOT(aceptar()));//Al pulsar aceptarButton se ejecuta el SLOT aceptar.
    connect(cancelarButton,SIGNAL(clicked()),this,SLOT(close()));//Al pulsar cancelarButton se ejecuta el SLOT close.

    setWindowTitle(tr("Añadir programa"));//El título de la ventana del diálogo es Añadir programa.
}
void dialogo_anadirprograma::actualizarProgramasAbiertos(){
    clear_windowInfoList();//Antes de rellenar la windowInfoList debemos limpiarla, para que no haya repeticiones del mismo programa.

    EnumWindows(EnumWindowsProc, NULL);//Rellenamos la windowInfoList.

    std::vector<WindowInfo> windowInfoList = get_windowInfoList();//Se hace una copia de windowInfoList.

    nProgramas = windowInfoList.size();//El número de programas a mostrar es el tamaño de la windowInfoList.

    QString nombreBoton[nProgramas];//El array nombreBoton almacenará los nombres que se les asignarán a los QCheckBox del diálogo.
    std::wstring programa;//Nombre temporal de cada programa.

    for(int i = 0;i < nProgramas;i++){//Por cada programa de la windowInfoList:
        programa = nombreProceso(windowInfoList[i].processName);//programa vale el nombre del proceso del programa de la windowInfoList.
        nombreBoton[i] = QString::fromStdWString(programa);//Casting de WString a QString, gracias a un método de la clase QString.
    }

    //Crearemos tantos QCheckBox como programas haya:

    for(int i = 0;i < nProgramas;i++){//Por cada programa, se crea su propio QCheckBox y se añade al layout.
        check[i] = new QCheckBox(nombreBoton[i]);//Se reserva memoria para el QCheckBox con el nombre deseado.
        boxLayout->addWidget(check[i]);//Añadimos este al boxLayout.
    }
}

//SLOT que guarda los programas seleccionados en un archivo.
void dialogo_anadirprograma::aceptar(){
    QFile archivoLectura("programas.dat");//Los programas deseados a monitorizar se almacenarán en el archivo programas.dat.
    if(!archivoLectura.open(QIODevice::ReadOnly)){//Si no se consigue abrir el archivo en modo solo lectura:
        //Se informa del error por consola:
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
    }

    QList<QString> programa;//programa es una lista de QString que almacenará los todos los programas a guardar.
    QString pr;//pr almacenará en cada iteración el programa a almacenar.

    QDataStream entrada(&archivoLectura);//entrada nos permite leer línea por línea el archivo.
    entrada.setVersion(QDataStream::Qt_6_4);//Se emplea la versión Qt_6_4 para ello.

    int i = 0;//Iterador i.
    bool cadenaVacia = false;//Si encontramos una cadena vacía, valdrá true.

    while(i < nMaxProgramas && !cadenaVacia){//Mientras no hayamos pasado el número máximo de programas ni hayamos encontrado cadena vacía:
        entrada>>pr;//pr vale la línea leída del archivo.
        if(pr == ""){//Si pr es cadena vacía, asignamos true a cadenaVacia:
            cadenaVacia = true;
        }else{//Si no:
            programa.push_back(pr);//Se añade a la lista de programas del archivo.
            i++;//Se incrementa el iterador.
        }
    }
    archivoLectura.close();//Cierra el archivo.

    QFile archivoEscritura("programas.dat");//Abriremos programas.dat en modo solo escritura.
    if(!archivoEscritura.open(QIODevice::WriteOnly)){//Si no se consigue abrir el archivo en solo escritura:
        //Se informa del error por consola:
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoEscritura.errorString());

        return;//Termina la ejecución, sin haberse podido escribir la lista de programas.
    }

    QDataStream salida(&archivoEscritura);
    salida.setVersion(QDataStream::Qt_6_4);

    //Iteramos cada check:
    for(int i = 0;i < nProgramas;i++){
        if(check[i]->isChecked() && !programa.contains(check[i]->text())){//Si check está checkado y su nombre no está en el archivo:
            programa.push_back(check[i]->text());//Se añade el nombre a la lista de programas del archivo.
        }
    }

    for(int i = 0;i < programa.size();i++){//Iteramos la lista programa:
        salida<<programa[i];//Se escribe cada programa en la salida (en el archivo programas.dat).
    }

    archivoEscritura.close();//Se cierra el fichero.
    close();//Se cierra el diálogo.
}

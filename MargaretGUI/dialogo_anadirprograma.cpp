#include "dialogo_anadirprograma.h"
#include "motor.h"//Muy importante incluir siempre solo los .h.
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

dialogo_anadirprograma::dialogo_anadirprograma(QWidget *parent) : QDialog(parent)
{
    //Importantísimo inicializar siempre las variables.
    aceptarButton = new QPushButton(tr("&Aceptar"));
    cancelarButton = new QPushButton(tr("&Cancelar"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    nMaxProgramas = 50;//Pongamos 50 por ejemplo.
    for(int i = 0;i < nMaxProgramas;i++){
        check[i] = nullptr;//Importantísimo inicializar los punteros a nullptr, sino no estarán inicializados y se ejecutarán métodos sin tener memoria reservada a pesar del control if(puntero == nullptr).
    }

    actualizarProgramasAbiertos();

    programasBox->setTitle(tr("Elija los programas que desee monitorizar"));
    programasBox->setLayout(boxLayout);

    horizontalLayout->addWidget(cancelarButton);
    horizontalLayout->addWidget(aceptarButton);
    verticalLayout->addWidget(programasBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);

    setAttribute(Qt::WA_DeleteOnClose);//Al ponerle este atributo al diálogo, cuando se cierre se destruirá.

    connect(aceptarButton,SIGNAL(clicked()),this,SLOT(aceptar()));
    connect(cancelarButton,SIGNAL(clicked()),this,SLOT(close()));

    //qDebug()<<get_windowInfoList()[1].processName;

    //Falta hacer un clear de windowInfo, ya veré cómo se hace. De momento no se actualizan los programas cuando se cierra y se abre el diálogo de nuevo.

    /*std::vector<WindowInfo> windowInfoList = get_windowInfoList();//Hacemos copia de windowInfoList.

    int nProgramas = windowInfoList.size();

    QString nombreBoton[nProgramas];//Este array almacenará los nombres que se les asignarán a los botones checkables del diálogo.
    std::wstring programa;//En cada iteración guardaremos a partir de la ruta completa que windowInfoList[i].processName nos ofrece, el nombre del programa usando la función nombreProceso de motor.h.

    for(int i = 0;i < nProgramas;i++){
        programa = nombreProceso(windowInfoList[i].processName);
        nombreBoton[i] = QString::fromStdWString(programa);//Pasamos de WString a QString directamente, gracias a un método de la clase QString.
    }

    //Crearemos tantos checkbox como programas haya:
    QCheckBox *check[nProgramas];

    for(int i = 0;i < nProgramas;i++){//Por cada programa, crearé su propio botón check y se lo asignaré al layout.
        check[i] = new QCheckBox(nombreBoton[i]);
        boxLayout->addWidget(check[i]);//Lo añadimos al layout.
    }

    programasBox->setTitle(tr("Elija los programas que desee monitorizar"));
    programasBox->setLayout(boxLayout);

    horizontalLayout->addWidget(cancelarButton);
    horizontalLayout->addWidget(aceptarButton);
    verticalLayout->addWidget(programasBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);*/
}
void dialogo_anadirprograma::actualizarProgramasAbiertos(){
    //Ya se actualiza pero creamos demasiados checkboxes. Cuando se cierre el diálogo, deben eliminarse del layout las cosas. Entonces se arreglará todo.
    clear_windowInfoList();//Antes de rellenar la windowInfoList debemos limpiarla, para que no haya repeticiones del mismo programa.

    EnumWindows(EnumWindowsProc, NULL);//Rellenamos la windowInfoList.

    //qDebug()<<get_windowInfoList().size();

    std::vector<WindowInfo> windowInfoList = get_windowInfoList();//Hacemos copia de windowInfoList.

    //qDebug()<<windowInfoList.size();

    nProgramas = windowInfoList.size();

    QString nombreBoton[nProgramas];//Este array almacenará los nombres que se les asignarán a los botones checkables del diálogo.
    std::wstring programa;//En cada iteración guardaremos a partir de la ruta completa que windowInfoList[i].processName nos ofrece, el nombre del programa usando la función nombreProceso de motor.h.

    for(int i = 0;i < nProgramas;i++){
        programa = nombreProceso(windowInfoList[i].processName);
        nombreBoton[i] = QString::fromStdWString(programa);//Pasamos de WString a QString directamente, gracias a un método de la clase QString.
    }

    //Crearemos tantos checkbox como programas haya:

    for(int i = 0;i < nProgramas;i++){//Por cada programa, crearé su propio botón check y se lo asignaré al layout.
        //check[i] = new QCheckBox(nombreBoton[i]);
        //boxLayout->addWidget(check[i]);//Lo añadimos al layout.
        if(check[i] == nullptr){//Solo reservamos memoria si no lo habíamos hecho antes. La reservamos con el texto indicado.
            check[i] = new QCheckBox(nombreBoton[i]);
            boxLayout->addWidget(check[i]);
        }else{//Si ya hay memoria reservada, simplemente cambiamos el texto.
            check[i]->setText(nombreBoton[i]);
        }
    }
    for(int i = nProgramas;i < nMaxProgramas;i++){//Este for es para eliminar el resto de checkboxes que queden fuera del nº de programas actual.
        if(check[i] != nullptr){//Si el checkbox tiene memoria reservada (si no la tiene no tenemos que hacer nada).
            check[i]->hide();//Lo ocultamos. Si no lo ocultamos seguirá apareciendo en el diálogo, a pesar de eliminarlo del layout (se vería feo además).
            boxLayout->removeWidget(check[i]);//Lo eliminamos del layout.
        }
    }
}
void dialogo_anadirprograma::closeEvent(QCloseEvent *event){
    //qDebug()<<"Debemos eliminar los widgets";
}
void dialogo_anadirprograma::aceptar(){
    QFile archivoLectura("programas.dat");
    if(!archivoLectura.open(QIODevice::ReadOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
        //Quitamos el return por si no existe.
    }
    /*
    En el array programa almacenaremos los programas que ya se encontraban
    antes en el fichero, y también almacenaremos los nuevos programas a añadir
    al fichero.
    */
    //QString programa[nMaxProgramas];
    QList<QString> programa;
    QString pr;
    QDataStream entrada(&archivoLectura);
    entrada.setVersion(QDataStream::Qt_6_4);
    qDebug()<<"----------------------------------------------";
    int i = 0;//Para iterar.
    bool cadenaVacia = false;//Si encontramos una cadena vacía, valdrá  true.
    while(i < nMaxProgramas && !cadenaVacia){
        //entrada>>programa[i];
        entrada>>pr;
        if(/*programa[i]*/pr == ""){
            cadenaVacia = true;
        }else{
            programa.push_back(pr);
            i++;
        }
    }
    //j contendrá el índice de la primera cadena vacía.
    for(int j = 0;j < i;j++){
        qDebug()<<programa[j];
    }
    qDebug()<<"----------------------------------------------";
    archivoLectura.close();

    QFile archivoEscritura("programas.dat");
    if(!archivoEscritura.open(QIODevice::WriteOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoEscritura.errorString());
        return;
    }
    QDataStream salida(&archivoEscritura);
    salida.setVersion(QDataStream::Qt_6_4);
    for(int i = 0;i < nProgramas;i++){
        if(check[i]->isChecked() && !programa.contains(check[i]->text())){
            //Si el check está checkado y no está en los programas anteriores...
            //salida<<check[i]->text();
            programa.push_back(check[i]->text());
        }
    }
    for(int i = 0;i < programa.size();i++){
        salida<<programa[i];
    }
    archivoEscritura.close();
    close();
}

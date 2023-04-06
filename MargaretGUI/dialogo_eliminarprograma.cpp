#include "dialogo_eliminarprograma.h"
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

dialogo_eliminarprograma::dialogo_eliminarprograma(QWidget *parent) : QDialog(parent)
{
    //Importantísimo inicializar siempre las variables.
    aceptarButton = new QPushButton(tr("&Aceptar"));
    cancelarButton = new QPushButton(tr("&Cancelar"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    nMaxProgramas = 50;//Pongamos 50 por ejemplo.
    nProgramas = 0;//Inicializamos a 0 nProgramas.
    for(int i = 0;i < nMaxProgramas;i++){
        check[i] = nullptr;//Importantísimo inicializar los punteros a nullptr, sino no estarán inicializados y se ejecutarán métodos sin tener memoria reservada a pesar del control if(puntero == nullptr).
    }

    actualizarProgramasArchivo();

    programasBox->setTitle(tr("Seleccione los programas que desee eliminar"));
    programasBox->setLayout(boxLayout);

    horizontalLayout->addWidget(cancelarButton);
    horizontalLayout->addWidget(aceptarButton);
    verticalLayout->addWidget(programasBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);

    setAttribute(Qt::WA_DeleteOnClose);//Al ponerle este atributo al diálogo, cuando se cierre se destruirá.

    connect(aceptarButton,SIGNAL(clicked()),this,SLOT(aceptar()));
    connect(cancelarButton,SIGNAL(clicked()),this,SLOT(close()));
}
void dialogo_eliminarprograma::actualizarProgramasArchivo(){
    int i = 0;
    bool cadenaVacia = false;//Si encontramos una cadena vacía, valdrá true.
    QString pr;
    QList<QString> programa;
    QFile archivoLectura("programas.dat");
    if(!archivoLectura.open(QIODevice::ReadOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
        return;
    }
    QDataStream entrada(&archivoLectura);
    entrada.setVersion(QDataStream::Qt_6_4);
    while(!cadenaVacia && i< nMaxProgramas){
        entrada>>pr;
        if(pr == ""){//Si la cadena es vacía.
            cadenaVacia = true;
        }else{//Solo pushearemos la cadena si esta no está vacía.
            programa.push_back(pr);
            i++;
        }
    }

    nProgramas = programa.size();//nProgramas valdrá el nº de elementos de programa.

    QString nombreBoton[nProgramas];//Este array almacenará los nombres que se les asignarán a los botones checkables del diálogo.

    for(int i = 0;i < nProgramas;i++){
        nombreBoton[i] = programa[i];//A cada botón le asignaremos el nombre del programa.
    }

    //Crearemos tantos checkbox como programas haya:
    for(int i = 0;i < nProgramas;i++){//Por cada programa, crearé su propio botón check y se lo asignaré al layout.
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
void dialogo_eliminarprograma::aceptar(){
/*
programa es una lista de QString. Este almacenará los programas que se
sobreescribirán en el archivo (es decir, todos los programas que permanecerán tras
eliminar los programas deseados).
*/
    QList<QString> programa;

    for(int i = 0;i < nProgramas;i++){
        if(!check[i]->isChecked()){//Si no está checkado lo añadimos al archivo.
            programa.push_back(check[i]->text());
        }
    }

    QFile archivoEscritura("programas.dat");
    if(!archivoEscritura.open(QIODevice::WriteOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoEscritura.errorString());
        return;
    }
    QDataStream salida(&archivoEscritura);
    salida.setVersion(QDataStream::Qt_6_4);

    for(int i = 0;i < programa.size();i++){
        salida<<programa[i];
    }

    archivoEscritura.close();
    close();
}

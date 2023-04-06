#include "ventanaprincipal.h"
#include "dialogo_anadirprograma.h"
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QHBoxLayout>
#include <QWidget>
#include <QAction>
#include <QApplication>
#include <QMenuBar>

VentanaPrincipal::VentanaPrincipal()
{
    verticalLayout = new QVBoxLayout;
    fechaLabel = new QLabel;
    fecha = new QDate;
    fechaCadena = new QString;
    transformador = new QLocale;
    //qDebug()<<transformador->toString(12);
    listaProgramas = new QLabel;
    contadoresProgramas = new QLabel;
    horizontalLayout = new QHBoxLayout;
    centralWidget = new QWidget;
    dialogoAdd = nullptr;//Importantísimo inicializarlo a nullptr.

    int dia = 0,mes = 0,anio = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);

    *fechaCadena = "<center><font color='black' size=5 face=arial><b>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</b></font></center>";
    fechaLabel->setText(*fechaCadena);
    listaProgramas->setText("Chrome <br> TikTok");
    contadoresProgramas->setText("3 h 21 min <br> 29 min 40 s");


    verticalLayout->addWidget(fechaLabel);
    horizontalLayout->addWidget(listaProgramas);
    horizontalLayout->addSpacing(100);//Lo ponemos justo en medio de listaProgramas y contadoresProgramas.
    horizontalLayout->addWidget(contadoresProgramas);
    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout->setAlignment(Qt::AlignHCenter);

    createActions();
    createMenus();

    centralWidget->setLayout(verticalLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Margaret");

    monitor = new Monitor(this);
    monitor->start();
}
//Abriremos el diálogo de añadir programas.
void VentanaPrincipal::anadirPrograma(){
    /*if(!dialogoAdd){
        //qDebug("No existe el dialogo");
        dialogoAdd = new dialogo_anadirprograma(this);  //Solo reservamos memoria si no habíamos reservado antes. Si ya hemos reservado, no reservamos más y así no tendremos un posible memory leak.
    }else{
        //qDebug("Ya sí existe el diálogo");
        dialogoAdd->actualizarProgramasAbiertos();
    }*/
    /*
    Ya podemos reservar memoria siempre sin preocuparnos, cuando cerramos
    el diálogo este mismo es destruido. Esto es gracias a setAttribute(...)
    en el constructor de dialogo_anadirprograma.cpp.
    */
    dialogoAdd = new dialogo_anadirprograma(this);
    dialogoAdd->show();
}
void VentanaPrincipal::eliminarPrograma(){
    dialogoRemove = new dialogo_eliminarprograma(this);
    dialogoRemove->show();
}
void VentanaPrincipal::createActions(){
    anadirProgramaAction = new QAction("&Añadir programa",this);
    eliminarProgramaAction = new QAction("&Eliminar programa",this);
    aboutQtAction = new QAction("Acerca de &Qt",this);
    ayudaMargaretAction = new QAction("&Documentación de Margaret",this);

    connect(anadirProgramaAction,SIGNAL(triggered()),this,SLOT(anadirPrograma()));
    connect(eliminarProgramaAction,SIGNAL(triggered()),this,SLOT(eliminarPrograma()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void VentanaPrincipal::createMenus(){
    programasMenu = menuBar()->addMenu(tr("&Programas"));
    programasMenu->addAction(anadirProgramaAction);
    programasMenu->addAction(eliminarProgramaAction);
    ayudaMenu = menuBar()->addMenu(tr("&Ayuda"));
    ayudaMenu->addAction(ayudaMargaretAction);
    ayudaMenu->addAction(aboutQtAction);
}

void VentanaPrincipal::setlistaProgramas(QString lProgramas){
    listaProgramas->setText(lProgramas);//Le asigna a listaProgramas la cadena lProgramas.
}

void VentanaPrincipal::setcontadoresProgramas(QString cProgramas){
    contadoresProgramas->setText(cProgramas);
}

void VentanaPrincipal::closeEvent(QCloseEvent *event){

}

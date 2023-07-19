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
#include <QTableWidget>
#include <QStringList>

VentanaPrincipal::VentanaPrincipal()
{
    verticalLayout = new QVBoxLayout;
    fechaLabel = new QLabel;
    fecha = new QDate;
    fechaCadena = new QString;
    fechaEstilo = new QString;
    transformador = new QLocale;
    //qDebug()<<transformador->toString(12);
    listaProgramas = new QLabel;
    contadoresProgramas = new QLabel;
    horizontalLayout = new QHBoxLayout;
    centralWidget = new QWidget;
    dialogoAdd = nullptr;//Importantísimo inicializarlo a nullptr.
    tabla = new QTableWidget;
    i = 0;

    int dia = 0,mes = 0,anio = 0, nProgramasDeseados = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);

    *fechaCadena = "<h1><center>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</center></h1>";
    *fechaEstilo = "color: #2d93ca; font: lighter;";

    fechaLabel->setText(*fechaCadena);
    fechaLabel->setStyleSheet(*fechaEstilo);

    listaProgramas->setText("Chrome <br> TikTok");
    contadoresProgramas->setText("3 h 21 min <br> 29 min 40 s");

    verticalLayout->addWidget(fechaLabel);
    horizontalLayout->addWidget(listaProgramas);
    horizontalLayout->addSpacing(100);//Lo ponemos justo en medio de listaProgramas y contadoresProgramas.
    horizontalLayout->addWidget(contadoresProgramas);
    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout->setAlignment(Qt::AlignHCenter);
    verticalLayout->addWidget(tabla);

    createActions();
    createMenus();

    centralWidget->setLayout(verticalLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Margaret");
    setWindowIcon(QIcon("./moon.png"));

    monitor = new Monitor(this);
    monitor->start();

    tabla->showGrid();
    tabla->setColumnCount(2);
    nProgramasDeseados = monitor->get_nProgramas();
    tabla->setRowCount(nProgramasDeseados);//El número de filas de la tabla será igual al nº de programas deseados a monitorizar.
    //Debemos completar lo de debajo:
}
//Abriremos el diálogo de añadir programas.
void VentanaPrincipal::anadirPrograma(){
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

void VentanaPrincipal::setFecha(QDate *fecha){
    int dia = 0,mes = 0,anio = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);
    *fechaCadena = "<center><font color='black' size=5 face=arial><b>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</b></font></center>";
}



void VentanaPrincipal::closeEvent(QCloseEvent *event){

}

#include "ventanaprincipal.h"
#include "dialogo_anadirprograma.h"
#include <QCloseEvent>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QHBoxLayout>
#include <QWidget>


VentanaPrincipal::VentanaPrincipal()
{
    verticalLayout = new QVBoxLayout;
    fechaLabel = new QLabel;
    anadirPr = new QPushButton;
    eliminarPr = new QPushButton;
    fecha = new QDate;
    fechaCadena = new QString;
    transformador = new QLocale;
    //qDebug()<<transformador->toString(12);
    listaProgramas = new QLabel;
    contadoresProgramas = new QLabel;
    horizontalLayout = new QHBoxLayout;
    centralWidget = new QWidget;

    int dia = 0,mes = 0,anio = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);

    *fechaCadena = "<center><font color='black' size=5 face=arial><b>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</b></font></center>";

    fechaLabel->setText(*fechaCadena);
    anadirPr->setText("&Añadir programa");
    eliminarPr->setText("&Eliminar programa");
    connect(anadirPr,SIGNAL(clicked()),
            this,SLOT(anadirPrograma()));
    connect(eliminarPr,SIGNAL(clicked()),
            this,SLOT(close()));


    listaProgramas->setText("Minecraft <br> TikTok");
    contadoresProgramas->setText("12 horas <br> 3 horas");


    verticalLayout->addWidget(fechaLabel);
    horizontalLayout->addWidget(listaProgramas);
    horizontalLayout->addSpacing(50);
    horizontalLayout->addWidget(contadoresProgramas);
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(anadirPr);
    verticalLayout->addWidget(eliminarPr); //Esto funciona con QDialog, no con QMainWindow.

    centralWidget->setLayout(verticalLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Margaret");
}
//Abriremos la ventana de añadir uno o varios programas que se encuentren abiertos
void VentanaPrincipal::anadirPrograma(){

}
void VentanaPrincipal::closeEvent(QCloseEvent *event){
    qDebug()<<"Has salido de la aplicación";
    qDebug()<<"Yujuuu";
}

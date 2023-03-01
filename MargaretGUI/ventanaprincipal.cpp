#include "ventanaprincipal.h"
#include <QCloseEvent>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QMap>

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

    int dia = 0,mes = 0,anio = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);
    QMap<int,QString> mapa = {//Mapea el número del mes con el nombre.
                {1,"enero" },
                {2,"febrero" },
                {3,"marzo" },
                {4,"abril" },
                {5,"mayo" },
                {6,"junio" },
                {7,"julio" },
                {8,"agosto" },
                {9,"septiembre" },
                {10,"octubre" },
                {11,"noviembre" },
                {12,"diciembre" }
    };

    *fechaCadena = "A dia " + transformador->toString(dia) + " de " + mapa.take(mes) + " de " + transformador->toString(anio);

    fechaLabel->setText(*fechaCadena);
    anadirPr->setText("Añadir programa");
    eliminarPr->setText("Eliminar programa");
    connect(anadirPr,SIGNAL(clicked()),
            this,SLOT(close()));
    connect(eliminarPr,SIGNAL(clicked()),
            this,SLOT(close()));
    verticalLayout->addWidget(fechaLabel);
    verticalLayout->addWidget(anadirPr);
    verticalLayout->addWidget(eliminarPr);
    setLayout(verticalLayout);
}
void VentanaPrincipal::closeEvent(QCloseEvent *event){
    qDebug()<<"Has salido de la aplicación";
    qDebug()<<"Yujuuu";
}

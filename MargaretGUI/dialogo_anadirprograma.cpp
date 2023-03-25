#include "dialogo_anadirprograma.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QGroupBox>

dialogo_anadirprograma::dialogo_anadirprograma(QWidget *parent) : QDialog(parent)
{
    aceptarButton = new QPushButton(tr("Aceptar"));
    cancelarButton = new QPushButton(tr("Cancelar"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;

    programasBox->setTitle(tr("Elija los programas que desee monitorizar"));


    horizontalLayout->addWidget(cancelarButton);
    horizontalLayout->addWidget(aceptarButton);
    verticalLayout->addWidget(programasBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);
}

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

dialogo_anadirprograma::dialogo_anadirprograma(QWidget *parent) : QDialog(parent)
{    
    aceptarButton = new QPushButton(tr("Aceptar"));
    cancelarButton = new QPushButton(tr("Cancelar"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;
    boxLayout = new QVBoxLayout;

    actualizarProgramasAbiertos();

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
    //También debo quitar lo de escribirPtos().
    escribirPtos();
    EnumWindows(EnumWindowsProc, NULL);

    //qDebug()<<get_windowInfoList().size();

    std::vector<WindowInfo> windowInfoList = get_windowInfoList();//Hacemos copia de windowInfoList.

    qDebug()<<windowInfoList.size();

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

    setLayout(verticalLayout);
}

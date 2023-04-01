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
    //Importantísimo inicializar siempre las variables, por estúpido que parezca.
    aceptarButton = new QPushButton(tr("Aceptar"));
    cancelarButton = new QPushButton(tr("Cancelar"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programasBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    for(int i = 0;i < 50;i++){
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

    int nProgramas = windowInfoList.size();

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
    for(int i = nProgramas;i < 50;i++){//Este for es para eliminar el resto de checkboxes que queden fuera del nº de programas actual.
        if(check[i] != nullptr){//Si el checkbox tiene memoria reservada (si no la tiene no tenemos que hacer nada).
            check[i]->hide();//Lo ocultamos. Si no lo ocultamos seguirá apareciendo en el diálogo, a pesar de eliminarlo del layout (se vería feo además).
            boxLayout->removeWidget(check[i]);//Lo eliminamos del layout.
        }
    }
}
void dialogo_anadirprograma::closeEvent(QCloseEvent *event){
    qDebug()<<"Debemos eliminar los widgets";
}

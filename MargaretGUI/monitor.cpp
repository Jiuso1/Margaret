#include "windowinfo.h"
#include "ventanaprincipal.h"
#include "motor.h"

#include <QDebug>
#include <windows.h>
#include <string>
#include <QLocale>
#include <QFile>

Monitor::Monitor(VentanaPrincipal *v)
{
    vPrincipal = v;//Hacemos que vPrincipal apunte a v (a la ventana principal original).
    transformador = new QLocale;//Reservamos memoria para el transformador.
    nMaxProgramas = 50;//En algún momento lo sincronizaremos con el resto.
    nProgramas = 0;
    espera = 10;//Tendremos una espera de 10 segundos.
    for(int i = 0;i < nMaxProgramas;i++){//Debería estar en función de nMaxProgramas la declaración de tiempoPrograma.
        tiempoPrograma[i] = 0;
    }
}
void Monitor::run(){
    /*Debemos leer los programas de programas.dat. Tendremos tantos contadores
    como programas haya.*/
    int i = 0;
    bool cadenaVacia = false;//Si encontramos una cadena vacía, valdrá true.
    QString pr;
    QFile archivoLectura("programas.dat");
    if(!archivoLectura.open(QIODevice::ReadOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
        return;
    }
    QDataStream entrada(&archivoLectura);
    entrada.setVersion(QDataStream::Qt_6_4);
    while(!cadenaVacia && i < nMaxProgramas){
        entrada>>pr;
        if(pr == ""){//Si la cadena es vacía.
            cadenaVacia = true;
        }else{//Solo pushearemos la cadena si esta no está vacía.
            programaArchivo.push_back(pr);
            i++;
        }
    }
    archivoLectura.close();
    nProgramas = programaArchivo.size();

    QString lProgramas = "";
    QString cProgramas ; "";

    for(int i = 0;i < nProgramas;i++){
        lProgramas += "<br>" + programaArchivo[i];
        qDebug()<<nProgramas;
    }

    vPrincipal->setlistaProgramas(lProgramas);

    std::vector<WindowInfo> windowInfoList;
    if(vPrincipal != nullptr){
        while(true){
            clear_windowInfoList();
            EnumWindows(EnumWindowsProc,NULL);
            windowInfoList = get_windowInfoList();
            for(int i = 0;i < nProgramas;i++){
                qDebug()<<programaArchivo[i]<<":"<<tiempoPrograma[i];
                if(contains(windowInfoList,programaArchivo[i].toStdString())){
                    //qDebug()<<programaArchivo[i];
                    tiempoPrograma[i] += espera;
                }
            }
            sleep(espera);
        }
    }
}

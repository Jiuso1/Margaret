#include "windowinfo.h"
#include "ventanaprincipal.h"
#include "motor.h"

#include <QDebug>
#include <windows.h>
#include <string>
#include <QLocale>
#include <QFile>
#include <QDate>

Monitor::Monitor(VentanaPrincipal *v)
{
    vPrincipal = v;//Hacemos que vPrincipal apunte a v (a la ventana principal original).
    transformador = new QLocale;//Reservamos memoria para el transformador.
    nMaxProgramas = 50;//En algún momento lo sincronizaremos con el resto.
    nProgramas = 0;
    espera = 10;//Tendremos una espera de 10 segundos.
    fecha = new QDate;
    for(int i = 0;i < nMaxProgramas;i++){//Debería estar en función de nMaxProgramas la declaración de tiempoPrograma.
        tiempoPrograma[i] = 0;
    }
}

void Monitor::run(){
    int dia = 0, diaPrevio = 0;//Almacenaremos el día actual y el día previo en cada momento. Si son diferentes ambos días, es que hemos cambiado de día.
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

    QStringList contador;
    int segundos = 0, minutos = 0, horas = 0;

    vPrincipal->setPrograma(programaArchivo);

    std::vector<WindowInfo> windowInfoList;
    if(vPrincipal != nullptr){
        while(true){
            fecha->currentDate().getDate(NULL,NULL,&dia);
            if(dia != diaPrevio){
                vPrincipal->setFecha(fecha);
                diaPrevio = dia;
                for(int i = 0;i < nProgramas;i++){
                    tiempoPrograma[i] = 0;
                    contador.append("0");//Añadimos tantos contadores como programas haya. Inicializamos a la cadena "0".
                }
            }
            clear_windowInfoList();
            EnumWindows(EnumWindowsProc,NULL);
            windowInfoList = get_windowInfoList();
            for(int i = 0;i < nProgramas;i++){
                //qDebug()<<programaArchivo[i]<<":"<<tiempoPrograma[i];
                if(tiempoPrograma[i] < 60){//Si no ha pasado del minuto...
                    contador[i] = transformador->toString(tiempoPrograma[i]) + " s";
                }else if(tiempoPrograma[i] < 3600){//Si no ha pasado de la hora...
                    minutos = tiempoPrograma[i]/60;
                    segundos = tiempoPrograma[i]%60;//Hacer división a mano y se entenderá fácilmente.
                    contador[i] = transformador->toString(minutos) + " min " + transformador->toString(segundos) + " s";
                }else{//Ya no tenemos que comprobar si es menor que un día, puesto que Margaret monitoriza cada día. Un programa no puede tener más de un día en ejecución para Margaret.
                    horas = tiempoPrograma[i]/3600;
                    minutos = tiempoPrograma[i]/60 - horas*60;
                    contador[i] = transformador->toString(horas) + " h " + transformador->toString(minutos) + " min ";
                }
                if(contains(windowInfoList,programaArchivo[i].toStdString())){
                    //qDebug()<<programaArchivo[i];
                    tiempoPrograma[i] += espera;
                }
            }
            vPrincipal->setContador(contador);
            sleep(espera);
        }
    }
}

int Monitor::get_nProgramas(){
    return nProgramas;
}

bool Monitor::guardarContador(){
    bool guardado = false;
    int diaArchivo = 0, mesArchivo = 0, anhoArchivo = 0;

    QMap<QString,unsigned long long int> mapaContador;//Mapea cada programa con el contador correspondiente.

    for(int i = 0;i < nProgramas;i++){
        mapaContador.insert(programaArchivo[i],tiempoPrograma[i]);//Por cada programa añadido al archivo, almacenaremos su contador.
    }

    QFile archivoEscritura("contadores.dat");
    QDataStream salida(&archivoEscritura);

    if(!archivoEscritura.open(QIODevice::WriteOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoEscritura.errorString());
    }else{
        salida.setVersion(QDataStream::Qt_6_4);
        fecha->currentDate().getDate(&anhoArchivo, &mesArchivo, &diaArchivo);
        salida<<diaArchivo<<mesArchivo<<anhoArchivo<<mapaContador;//Escribimos la fecha de los contadores y el mapa.
        guardado = true;
        archivoEscritura.close();
    }

    return guardado;
}

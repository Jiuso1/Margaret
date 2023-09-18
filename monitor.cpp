#include "windowinfo.h"
#include "ventanaprincipal.h"
#include "motor.h"

#include <QDebug>
#include <windows.h>
#include <string>
#include <QLocale>
#include <QFile>
#include <QDate>

Monitor::Monitor(VentanaPrincipal *v){
    vPrincipal = v;//vPrincipal apunta a v (a la ventana principal original).
    transformador = new QLocale;//Reservamos memoria para el transformador.
    nMaxProgramas = 50;//En algún momento lo sincronizaremos con el resto.
    nProgramas = 0;//Inicializamos a 0.
    espera = 10;//Tendremos una espera de 10 segundos.
    fecha = new QDate;//Reservamos memoria para la fecha.
    for(int i = 0;i < nMaxProgramas;i++){//Debería estar en función de nMaxProgramas la declaración de tiempoPrograma.
        tiempoPrograma[i] = 0;//Inicializamos a 0 el tiempo de cada programa.
    }
}

//El constructor será igual que el de arriba, lo único nuevo es que guardará el mapa pasado por parámetro en el atributo correspondiente.
Monitor::Monitor(VentanaPrincipal *v, const QMap<QString,std::uint64_t> &mapaContador){
    vPrincipal = v;//vPrincipal apunta a v (a la ventana principal original).
    transformador = new QLocale;//Reservamos memoria para el transformador.
    nMaxProgramas = 50;//En algún momento lo sincronizaremos con el resto.
    nProgramas = 0;//Inicializamos a 0.
    espera = 10;//Tendremos una espera de 10 segundos.
    fecha = new QDate;//Reservamos memoria para la fecha.
    for(int i = 0;i < nMaxProgramas;i++){//Debería estar en función de nMaxProgramas la declaración de tiempoPrograma.
        tiempoPrograma[i] = 0;//Inicializamos a 0 el tiempo de cada programa.
    }
    this->mapaContador = mapaContador;//Le asignamos al mapa de la clase Monitor el mapa pasado por parámetro.
}

//Método que se ejecucuta concurrentemente con la respuesta de la GUI. Cuenta el tiempo de las aplicaciones.
void Monitor::run(){
    //Almacenaremos el día actual y el día previo en cada momento. Si son diferentes ambos días, es que hemos cambiado de día:
    int dia = 0;
    int diaPrevio = 0;

    int i = 0;//Iterador inicializado a 0.
    bool cadenaVacia = false;//Si encontramos una cadena vacía, valdrá true.
    QString pr;//Programa iterado.

    //Abrimos el archivo programas.dat, y si no da fallo su apertura continuamos en el código:
    QFile archivoLectura("programas.dat");
    if(!archivoLectura.open(QIODevice::ReadOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
        return;
    }

    //Leemos el contenido del archivo:
    QDataStream entrada(&archivoLectura);//La entrada de datos vendrá por archivoLectura (que tiene abierto programas.dat).
    entrada.setVersion(QDataStream::Qt_6_4);//La versión de Qt empleada para la lectura será Qt_6_4.
    //Mientras no haya cadena vacía y no se haya pasado el límite de programas máximos a monitorizar:
    while(!cadenaVacia && i < nMaxProgramas){
        entrada>>pr;//Guardamos el programa en pr.
        if(pr == ""){//Si la cadena es vacía.
            cadenaVacia = true;//cadenaVacia es verdadero.
        }else{//Si la cadena no es vacía.
            programaArchivo.push_back(pr);//Añadimos al conjunto de programas leídos del archivo dicha cadena.
            i++;//Incrementamos i para iterar.
        }
    }
    archivoLectura.close();//Cerramos el archivo.
    nProgramas = programaArchivo.size();//El número de programas deseados a monitorizar será igual al número de programas leídos del archivo.

    QStringList contador;//Contadores de cada programa.
    int segundos = 0, minutos = 0, horas = 0;//Segundos, minutos y horas inicializados a 0.

    vPrincipal->setPrograma(programaArchivo);//vPrincipal tendrá la lista de programas deseados a monitorizar.

    std::vector<WindowInfo> windowInfoList;//Conjunto que almacena estructuras WindowInfo (Nombre de la ventana,Nombre del proceso).
    if(vPrincipal != nullptr){//Si vPrincipal tiene memoria reservada.
        while(true){//Por siempre.
            fecha->currentDate().getDate(NULL,NULL,&dia);//Se asigna a dia el día actual.
            if(dia != diaPrevio){//Si el dia actual es diferente al diaPrevio (al dia almacenado anteriormente).
                *fecha = fecha->currentDate();//Actualizamos la fecha actual.
                vPrincipal->setFecha(fecha);//Actualizamos la fecha de la ventana.
                diaPrevio = dia;//Se asigna a diaPrevio el dia actual.
                for(int i = 0;i < nProgramas;i++){//Por cada programa.
                    tiempoPrograma[i] = 0 + mapaContador[programaArchivo[i]];//El tiempo del programa será igual al tiempo registrado por Margaret en ese mismo día. Si no se ha usado el programa, el tiempo será 0.
                    contador.append("0");//Añadimos tantos contadores como programas haya. Inicializamos a la cadena "0".
                }
            }

            mapaContador.clear();//Cuando hemos terminado de usar el diccionario, lo eliminamos.
            clear_windowInfoList();//Eliminamos la lista de estructuras WindowInfo, para resetearla.
            EnumWindows(EnumWindowsProc,NULL);//Se llena la windowInfoList del motor. Esta solo tendrá nombres de procesos con ventanas abiertas.
            windowInfoList = get_windowInfoList();//Guardamos la windowInfoList del motor en la windowInfoList del monitor.

            for(int i = 0;i < nProgramas;i++){//Por cada programa.
                if(tiempoPrograma[i] < 60){//Si el programa lleva menos de un minuto.
                    contador[i] = transformador->toString(tiempoPrograma[i]) + " s";//Cadena formateada correctamente.
                }else if(tiempoPrograma[i] < 3600){//Si el programa lleva menos de una hora.
                    minutos = tiempoPrograma[i]/60;//Tiempo (segundos) / 60 = Tiempo (minutos).
                    segundos = tiempoPrograma[i]%60;//Tiempo (>60 segundos) % 60 = Tiempo (<60 segundos). Hacer división a mano y se entenderá fácilmente.
                    contador[i] = transformador->toString(minutos) + " min " + transformador->toString(segundos) + " s";//Cadena formateada correctamente.
                }else{//Ya no tenemos que comprobar si es menor que un día, puesto que Margaret monitoriza cada día. Un programa no puede tener más de un día en ejecución para Margaret.
                    horas = tiempoPrograma[i]/3600;//Tiempo (segundos) / 3600 = Tiempo (horas).
                    minutos = tiempoPrograma[i]/60 - horas*60;//(Tiempo (segundos) / 60) - (Tiempo(horas) * 60) = Tiempo (minutos) - Tiempo (minutos) = Tiempo (minutos)
                    contador[i] = transformador->toString(horas) + " h " + transformador->toString(minutos) + " min ";//Cadena formateada correctamente.
                }
                if(contains(windowInfoList,programaArchivo[i].toStdString())){//Si el programa i está en la windowInfoList.
                    tiempoPrograma[i] += espera;//Le sumamos la espera de tiempo a su tiempo de ejecución.
                }
            }
            vPrincipal->setContador(contador);//Actualizamos los contadores de la ventana principal.
            sleep(espera);//Esperamos la espera hasta la próxima iteración.
        }
    }
}

//Retorna el número de programas deseados a monitorizar.
int Monitor::get_nProgramas(){
    return nProgramas;//nProgramas vale el número de programas de programas.dat.
}

//Guarda los contadores de cada programa en el archivo contadores.dat
bool Monitor::guardarContador(){
    bool guardado = false;//guardado vale true si se han conseguido guardar todos los contadores.

    //En el archivo almacenamos la fecha de guardado, para que Margaret solo recoja los contadores si se encuentra en el mismo día.
    //Inicializamos a 0:
    int diaArchivo = 0;
    int mesArchivo = 0;
    int anhoArchivo = 0;

    QMap<QString,std::uint64_t> mapaContador;//Mapea cada programa con el contador correspondiente.

    for(int i = 0;i < nProgramas;i++){
        mapaContador.insert(programaArchivo[i],tiempoPrograma[i]);//Por cada programa añadido al archivo, almacenaremos su contador.
    }

    //Abrimos contadores.dat en escritura:
    QFile archivoEscritura("contadores.dat");
    QDataStream salida(&archivoEscritura);

    if(!archivoEscritura.open(QIODevice::WriteOnly)){
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoEscritura.errorString());//Se escribe un mensaje de error si ha fallado la apertura.
    }else{//Si no falla la apertura.
        salida.setVersion(QDataStream::Qt_6_4);//La versión de Qt empleada para escribir es Qt_6_4.
        fecha->currentDate().getDate(&anhoArchivo, &mesArchivo, &diaArchivo);//Asignamos a anhoArchivo, mesArchivo y diaArchivo los valores correspondientes de la fecha actual.
        salida<<diaArchivo<<mesArchivo<<anhoArchivo<<mapaContador;//Escribimos la fecha de los contadores y el mapa.
        guardado = true;//Se han guardado con éxito los contadores.
        archivoEscritura.close();//Cerramos el archivo.
   }

    return guardado;//Retornamos si se ha guardado o no.
}

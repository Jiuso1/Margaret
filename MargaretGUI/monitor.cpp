#include "windowinfo.h"
#include "ventanaprincipal.h"

#include <QDebug>
#include <windows.h>
#include <string>
#include <QLocale>

Monitor::Monitor(VentanaPrincipal *v)
{
    vPrincipal = v;//Hacemos que vPrincipal apunte a v (a la ventana principal original).
    transformador = new QLocale;//Reservamos memoria para el transformador.
}
void Monitor::run(){
    int contador = 0;
    while(true){
        if(vPrincipal != nullptr){
            vPrincipal->setlistaProgramas(transformador->toString(contador));
        }
        contador++;
        sleep(10);
    }
}

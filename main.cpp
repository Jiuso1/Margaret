#include <QApplication>
#include "ventanaprincipal.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//Se declara la aplicación pasándole los argumentos de la consola.
    VentanaPrincipal *ventana = new VentanaPrincipal;//Ventana principal a la que se le reserva memoria.
    ventana->show();//Se muestra la ventana.
    return a.exec();//Se ejecuta la aplicación.
}

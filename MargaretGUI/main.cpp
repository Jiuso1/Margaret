#include <QApplication>
#include "ventanaprincipal.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VentanaPrincipal *ventana = new VentanaPrincipal;
    ventana->show();
    return a.exec();
}

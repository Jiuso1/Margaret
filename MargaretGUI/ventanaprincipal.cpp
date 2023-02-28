#include "ventanaprincipal.h"
#include <QCloseEvent>
#include <QDebug>

VentanaPrincipal::VentanaPrincipal()
{

}
void VentanaPrincipal::closeEvent(QCloseEvent *event){
    qDebug()<<"Has salido de la aplicación";
    qDebug()<<"Yujuuu";
}

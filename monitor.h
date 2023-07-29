#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <vector>

class QString;
class VentanaPrincipal;
class QLocale;
class QDate;

class Monitor : public QThread  //El cálculo debe hacerse en paralelo con la respuesta de la interfaz.
{
private:
    QStringList programaArchivo;//Almacenará el conjunto de programas a monitorizar. Este conjunto estará guardado en programas.dat.
    VentanaPrincipal *vPrincipal;//Monitor modificará de la ventana principal los labels dedicados al conjunto de programas y sus contadores.
    QLocale *transformador;//Nos permitirá pasar de int a QString.
    int nMaxProgramas;
    int nProgramas;
    unsigned long long int tiempoPrograma[50];//En vez de 50 debería ser nMaxProgramas.
    //tiempoPrograma[i] almacenará el tiempo de ejecución en segundos del programa i.
    int espera;
    QDate *fecha;//Con fecha miraremos si cambia la fecha. Si cambia, reiniciamos los contadores y cambiamos la label de VPrincipal.
public:
    Monitor(VentanaPrincipal *v = nullptr);
    int get_nProgramas();
    bool guardarContador();

protected:
    void run();
};

#endif // MONITOR_H

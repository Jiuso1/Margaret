#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <vector>
#include <QList>

class QString;
class VentanaPrincipal;
class QLocale;

class Monitor : public QThread  //El cálculo debe hacerse en paralelo con la respuesta de la interfaz.
{
private:
    QList<QString> programaArchivo;//Almacenará el conjunto de programas a monitorizar. Este conjunto estará guardado en programas.dat.
    VentanaPrincipal *vPrincipal;//Monitor modificará de la ventana principal los labels dedicados al conjunto de programas y sus contadores.
    QLocale *transformador;//Nos permitirá pasar de int a QString.
public:
    Monitor(VentanaPrincipal *v = nullptr);

protected:
    void run();
};

#endif // MONITOR_H

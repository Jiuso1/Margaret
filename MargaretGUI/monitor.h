#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>

class Monitor : public QThread  //El cálculo debe hacerse en paralelo con la respuesta de la interfaz.
{
public:
    Monitor();
    void run();
};

#endif // MONITOR_H

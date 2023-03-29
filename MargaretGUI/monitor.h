#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <vector>

class Monitor : public QThread  //El cálculo debe hacerse en paralelo con la respuesta de la interfaz.
{
private:

public:
    Monitor();
    void run();
};

#endif // MONITOR_H

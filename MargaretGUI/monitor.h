#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <vector>
#include "windowinfo.h"
#include <windows.h>

struct WindowInfo;

class Monitor : public QThread  //El cálculo debe hacerse en paralelo con la respuesta de la interfaz.
{
private:
    std::vector<WindowInfo> windowInfoList;//Almacenará el nombre de los procesos que tengan una ventana abierta en cada momento.

public:
    Monitor();
    void run();
    std::vector<WindowInfo> get_windowInfoList();
    BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
    bool contains(std::vector<WindowInfo> vectorWI, std::string s);
    std::wstring nombreProceso(std::wstring cadenaCompleta);
};

#endif // MONITOR_H

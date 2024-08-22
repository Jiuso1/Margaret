#ifndef WINDOWINFO_H
#define WINDOWINFO_H

#include <string>//std::wstring incluido en string.

//Estructura WindowInfo:
struct WindowInfo {
    std::wstring windowTitle;//Nombre de la ventana.
    std::wstring processName;//Nombre del proceso.
};

#endif // WINDOWINFO_H

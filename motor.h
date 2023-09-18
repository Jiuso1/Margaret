#ifndef MOTOR_H
#define MOTOR_H

#include "windowinfo.h"

#include <windows.h>
#include <vector>
#include <string>

void clear_windowInfoList();//Elimina la variable global windowInfoList.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);//Llena la windowInfoList del motor.
std::vector<WindowInfo> get_windowInfoList();
std::wstring nombreProceso(std::wstring cadenaCompleta);
bool contains(std::vector<WindowInfo> vectorWI, std::string s);

#endif // MOTOR_H

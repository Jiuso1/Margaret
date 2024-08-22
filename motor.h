#ifndef MOTOR_H
#define MOTOR_H

#include "windowinfo.h"

#include <windows.h>
#include <vector>
#include <string>

void clear_windowInfoList();//Vacía la windowInfoList del motor.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);//Llena la windowInfoList del motor.
std::vector<WindowInfo> get_windowInfoList();//Devuelve la windowInfoList del motor.
std::wstring nombreProceso(std::wstring cadenaCompleta);//Devuelve el nombre de un proceso partiendo de su ruta completa.
bool contains(std::vector<WindowInfo> vectorWI, std::string s);//Devuelve true si vectorWI contiene algún nombre de proceso igual a s.

#endif // MOTOR_H

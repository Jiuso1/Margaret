#ifndef MOTOR_H
#define MOTOR_H

#include "windowinfo.h"

#include <windows.h>
#include <vector>

void clear_windowInfoList();
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
std::vector<WindowInfo> get_windowInfoList();
std::wstring nombreProceso(std::wstring cadenaCompleta);

#endif // MOTOR_H

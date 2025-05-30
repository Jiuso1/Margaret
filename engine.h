#ifndef ENGINE_H
#define ENGINE_H

#include "windowinfo.h"

#include <windows.h>
#include <vector>
#include <string>

void clear_windowInfoList();//Empty the windowInfoList from the engine.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);//Fills the engine's windowInfoList.
std::vector<WindowInfo> get_windowInfoList();//Returns the engine's windowInfoList.
std::wstring processName(std::wstring stringComplete);//Returns the name of a process starting from its complete path.
bool contains(std::vector<WindowInfo> vectorWI, std::string s);//Returns true if vectorWI contains any process name equal to s.

#endif // ENGINE_H

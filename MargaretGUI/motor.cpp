#include "motor.h"
#include "windowinfo.h"

#include <QDebug>
#include <windows.h>

std::vector<WindowInfo> windowInfoList;//Aquí nos crearemos windowInfoList, que almacenará en todo momento los procesos con ventanas abiertas.

void clear_windowInfoList(){
    windowInfoList.clear();
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowInfo info;
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    TCHAR buffer[MAX_PATH];
    GetWindowText(hwnd, buffer, MAX_PATH);
    //wcout << "Window title: " << buffer << endl;
    info.windowTitle = buffer;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess != NULL) {
        TCHAR processName[MAX_PATH];
        DWORD processNameSize = MAX_PATH;
        if (QueryFullProcessImageName(hProcess, 0, processName, &processNameSize)) {
            //wcout << "Process name: " << processName << endl;
            info.processName = processName;
        }
        CloseHandle(hProcess);
    }
    bool existe = false;
    for (int i = 0; i < windowInfoList.size(); i++) {
        if (info.processName == windowInfoList[i].processName) {
            existe = true;
            break;
        }
    }
    if (!existe) {
        if (IsWindowVisible(hwnd)) {
            windowInfoList.push_back(info);
        }
    }
    return TRUE;
}
std::vector<WindowInfo> get_windowInfoList(){
    return windowInfoList;
}
//A esta función le pasamos la cadena completa del proceso (C\..\proceso.exe) y nos devolverá el nombre del proceso.
std::wstring nombreProceso(std::wstring cadenaCompleta) {
    size_t pos = cadenaCompleta.find_last_of('\\') + 1;/*La posición de la primera letra del proceso será la de justo después
                                                     de la última barra. Como es back slash hace falta otra back slash.
                                                     Lo ponemos como size_t para evitar pérdida de datos al convertir a int.*/
    bool parar = false;
    std::wstring nProceso;
    while (parar == false) {
        if (cadenaCompleta[pos] == '.') {
            parar = true;
        }
        else {
            nProceso += cadenaCompleta[pos];
            pos++;
        }
    }
    return nProceso;
}

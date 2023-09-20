#include "motor.h"
#include "windowinfo.h"

#include <QDebug>
#include <windows.h>
//#include <iostream>

std::vector<WindowInfo> windowInfoList;//Declaramos windowInfoList en el motor.

//Vacía la windowInfoList del motor.
void clear_windowInfoList(){
    windowInfoList.clear();//Se eliminan los elemenos que windowInfoList contiene.
}

//Llena la windowInfoList del motor. Este método se llama tantas veces como procesos haya.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {//hwnd tiene el valor en hexadecimal del identificador de un proceso, pero no debe confundirse con el PID.
    WindowInfo info;//Almacenará del proceso que sea tanto su nombre como el nombre de su ventana.
    DWORD pid;//Almacenará el PID del proceso que sea.
    //std::wcout<<"Se le pasa a "<<"GetWindowThreadProcessId hwnd con valor "<<hwnd<<std::endl;
    GetWindowThreadProcessId(hwnd, &pid);//A pid se le asigna el PID del proceso que sea.
    TCHAR buffer[MAX_PATH];//Buffer que almacena caracteres.
    GetWindowText(hwnd, buffer, MAX_PATH);//buffer valdrá el nombre de la ventana del proceso con el id pasado por hwnd.
    info.windowTitle = buffer;//Guardamos en el atributo windowTitle de la info del proceso el nombre de su ventana.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);//OpenProcess nos retorna un manejador limitado del proceso, pasándole el PID.
    if (hProcess != NULL) {//Si hemos obtenido el manejador limitado del proceso.
        TCHAR processName[MAX_PATH];//Nombre del proceso.
        DWORD processNameSize = MAX_PATH;//Tamaño máximo del nombre del proceso (que coincide con el nº máximo de caracteres de un path).
        if (QueryFullProcessImageName(hProcess, 0, processName, &processNameSize)) {//processName valdrá el nombre del proceso.
            info.processName = processName;//Guardamos en el atributo processName de la info del proceso el nombre del proceso.
        }
        CloseHandle(hProcess);//Cerramos el manejador.
    }
    bool existe = false;//Variable que controlará si ya existe el proceso en la windowInfoList o no.
    for (int i = 0; i < windowInfoList.size(); i++) {//Por cada elemento de windowInfoList:
        if (info.processName == windowInfoList[i].processName) {//Si el nombre del proceso es igual que el nombre del elemento de la windowInfoList que estamos mirando:
            existe = true;//Sí existe.
            break;//Salimos del for. Ya sabemos que existe, no tenemos que comprobar más.
        }
    }
    if (!existe) {//Si no existe:
        if (IsWindowVisible(hwnd)) {//Si la ventana del proceso es visible:
            windowInfoList.push_back(info);//Se añade la info del proceso a la windowInfoList.
        }
    }
    return TRUE;//Retorna true.
}

//Devuelve la windowInfoList del motor.
std::vector<WindowInfo> get_windowInfoList(){
    return windowInfoList;//Retorna la windowInfoList.
}

//A esta función le pasamos la cadena completa del proceso (C\..\proceso.exe) y nos devolverá el nombre del proceso.
std::wstring nombreProceso(const std::wstring cadenaCompleta) {
    size_t pos = cadenaCompleta.find_last_of('\\') + 1;/*La posición de la primera letra del proceso será la de justo después
                                                     de la última barra. Como es back slash hace falta otra back slash por
                                                     sintaxis de C++.
                                                     pos es de tipo size_t para evitar pérdida de datos al convertir a int.*/
    bool parar = false;//Variable que controlará cuándo debe parar el bucle while.
    std::wstring nProceso;//Cadena que almacenará el nombre del proceso.
    while (parar == false) {//Mientras no haya que parar:
        if (cadenaCompleta[pos] == '.') {//Si el elemento de cadenaCompleta por el que vamos es un punto, ya debemos parar. Recuerda: C\..\proceso.exe
            parar = true;//Hay que parar.
        }
        else {//Si no:
            nProceso += cadenaCompleta[pos];//Añadimos al nombre del proceso el caracter que comparamos antes.
            pos++;//Se accederá a la siguiente posición.
        }
    }
    return nProceso;//Retorna el nombre del proceso.
}

//Devuelve true si vectorWI contiene algún elemento WindowInfo con atributo processName igual a s.
bool contains(std::vector<WindowInfo> vectorWI, const std::string s) {
    for (int i = 0; i < vectorWI.size(); i++) {//Por cada elemento de vectorWI:
        std::string processNameString(vectorWI[i].processName.begin(), vectorWI[i].processName.end());//Transforma wstring a string, almacenando en processNameString el nombre del proceso.
        if (processNameString.find(s) != std::string::npos) {//Si el nombre del proceso es igual a s:
            return true;//Retorna true.
        }
    }
    return false;//Retorna false.
}

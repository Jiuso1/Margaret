#include "engine.h"
#include "windowinfo.h"

#include <QDebug>
#include <windows.h>
#include <iostream>

std::vector<WindowInfo> windowInfoList;//Declare windowInfoList in the engine.

//Clear the windowInfoList from the engine.
void clear_windowInfoList(){
    windowInfoList.clear();//The elements that windowInfoList contains are removed.
}

//Fills the engine's windowInfoList. This method is called as many times as there are processes.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {//hwnd has the hexadecimal value of the identifier of a process, but should not be confused with the PID.
    WindowInfo info;//Stores both the name of the process and the name of its window.
    DWORD pid;//Stores the PID of any process.
    //std::wcout<<"Passed to"<<"GetWindowThreadProcessId hwnd with value"<<hwnd<<std::endl;
    GetWindowThreadProcessId(hwnd, &pid);//A pid is assigned the PID of whatever process.
    TCHAR buffer[MAX_PATH];//Buffer that stores characters.
    GetWindowText(hwnd, buffer, MAX_PATH);//buffer will be worth the name of the process window with the id passed by hwnd.
    info.windowTitle = buffer;//Buffer will store in the windowTitle attribute of the process info the name of its window.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);//OpenProcess returns a limited process handle, passing it the PID.
    if (hProcess != NULL) {//If we have obtained the limited process handle.
        TCHAR processName[MAX_PATH];//Name of the process.
        DWORD processNameSize = MAX_PATH;//Maximum size of the process name (which matches the maximum number of characters in a path).
        if (QueryFullProcessImageName(hProcess, 0, processName, &processNameSize)) {///processName will be worth the process name.
            info.processName = processName;//We store in the processName attribute of the process info the name of the process.
            //std::wcout<<processName<<std::endl;
        }
        CloseHandle(hProcess);//Close the handler.
    }
    bool exists = false;//Variable that will control if the process already exists in the windowInfoList or not.
    for (int i = 0; i < windowInfoList.size(); i++) {//For each element of windowInfoList:
        if (info.processName == windowInfoList[i].processName) {//If the process name is the same as the name of the windowInfoList item we are looking at:
            exists = true;//If it exists.
            break;//We exit the for. We already know it exists, we don't have to check anymore.
        }
    }
    if (!exists) {//If it doesn't exist:
        if (IsWindowVisible(hwnd)) {//If the process window is visible:
            windowInfoList.push_back(info);//The process info is added to the windowInfoList.
        }
    }
    return TRUE;//Return true.
}

//Returns the windowInfoList from the engine.
std::vector<WindowInfo> get_windowInfoList(){
    return windowInfoList;//Returns the windowInfoList.
}

//This function we pass the full string of the process (C\process.exe) and it will return the name of the process.
std::wstring processName(const std::wstring stringComplete) {
    size_t pos = stringComplete.find_last_of('\\') + 1;/*The position of the first letter of the process will be the one just after the last slash.
                                                     the last slash. Since it is a back slash, another back slash is required by
                                                     C++ syntax.
                                                     pos is of type size_t to avoid data loss when converting to int.*/
    bool stop = false;//Variable that will control when to stop the while loop.
    std::wstring nProcess;//String that will store the process name.
    while (stop == false) {//While there is no need to stop:
        if (stringComplete[pos] == '.') {//If the element of stringComplete we are going by is a dot, we should already stop. Remember:
            stop = true;//We must stop.
        }
        else {//If not:
            nProcess += stringComplete[pos];//We add to the process name the character we compared before.
            pos++;//The next position will be accessed.
        }
    }
    return nProcess;//Return the process name.
}

//Returns true if vectorWI contains any WindowInfo element with processName attribute equal to s.
bool contains(std::vector<WindowInfo> vectorWI, const std::string s) {
    for (int i = 0; i < vectorWI.size(); i++) {//For each element of vectorWI:
        std::string processNameString(vectorWI[i].processName.begin(), vectorWI[i].processName.end());//Transforms wstring to string, storing in processNameString the process name.
        if (processNameString.find(s) != std::string::npos) {//If the process name is equal to s:
            return true;//Return true.
        }
    }
    return false;//Return false.
}

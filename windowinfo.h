#ifndef WINDOWINFO_H
#define WINDOWINFO_H

#include <string> // std::wstring is included via string.

// WindowInfo structure:
struct WindowInfo {
    std::wstring windowTitle;  // Window title.
    std::wstring processName;  // Process name.
};

#endif // WINDOWINFO_H

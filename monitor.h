#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <QMap> // Included because it's a template class and causes issues with forward declarations.

// Forward declarations:
class QString;
class MainWindow;
class QLocale;
class QDate;

class Monitor : public QThread  // Monitor is a subclass of QThread. It runs independently from the GUI thread.
{
private:
    QStringList programFile; // Stores the list of programs to monitor, loaded from programs.dat.
    MainWindow *mainWindow; // Pointer to the main window. The monitor updates the main window.
    QLocale *transformer; // Used to convert int to QString using toString().
    int maxPrograms; // Maximum number of programs that can be monitored.
    int nPrograms; // Number of programs to monitor.
    std::uint64_t programTime[50]; // Should ideally be maxPrograms. Stores execution time in seconds for each program.
    int waitTime; // Monitor's wait time in seconds. Lower values increase precision but also resource usage.
    QDate *date; // Current date.
    QMap<QString, std::uint64_t> counterMap; // Map passed from the main window. Format: <program, counter>.

public:
    Monitor(MainWindow *w = nullptr); // Constructor.
    Monitor(MainWindow *w, const QMap<QString, std::uint64_t> &counterMap); // Constructor with counters, used when the date hasn't changed.
    int get_nPrograms(); // Returns the number of programs to monitor.
    bool saveCounter(); // Saves the counter map to a file. Returns true if successful.

protected:
    void run(); // Method executed concurrently with the GUI. Tracks application runtime.
};

#endif // MONITOR_H

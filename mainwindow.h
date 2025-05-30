#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "monitor.h"
#include "dialog_addprogram.h"
#include "dialog_deleteprogram.h"

// Forward declarations:
class QCloseEvent;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QDate;
class QLocale;
class QString;
class QWidget;
class QAction;
class QMenu;
class QTableWidget;

class MainWindow : public QMainWindow // MainWindow is a main window (inherits from QMainWindow).
{
    Q_OBJECT // Required because we use SIGNAL and SLOT.

public:
    MainWindow(); // Constructor.
    void createActions(); // Creates the application actions.
    void createMenus(); // Creates the menus and adds actions to them.
    void setDate(QDate *date); // Sets the QDate passed as a parameter to dateLabel, using the Day/Month/Year format.
    void setProgram(const QStringList &program); // Sets the QStringList passed as a parameter to the program list.
    void setCounter(const QStringList &counter); // Sets the QStringList passed as a parameter to the counter list.
    bool saveCounter(); // Saves the counters to a file by calling a method from the monitor.
    QMap<QString, std::uint64_t> readCounter(); // Reads a map<program, counter> from a file and returns it. If the date in the file differs from the current date, returns an empty map.
    void allocateMemory(); // Allocates memory for pointer attributes, excluding menus and actions.

protected:
    void closeEvent(QCloseEvent *event); // Triggered when the application is closed. Saves the counters to a file.

private slots:
    void addProgram(); // Opens the addProgram dialog.
    void deleteProgram(); // Opens the deleteProgram dialog.
    void aboutMargaret(); // Opens a window with information about Margaret.

private:
    QLabel *dateLabel; // Displays the current date.
    QVBoxLayout *verticalLayout; // Arranges dateLabel and table vertically.
    QDate *date; // Stores the current system date using currentDate().
    QLocale *transformer; // Converts int to QString using toString().
    QString *dateString; // Stores the current date as a string.
    QWidget *centralWidget; // Central widget containing the main layout.
    QAction *addProgramAction; // Linked to the addProgram() slot.
    QAction *deleteProgramAction; // Linked to the deleteProgram() slot.
    QAction *aboutQtAction; // Linked to the aboutQt() slot.
    QAction *helpMargaretAction; // PENDING.
    QAction *aboutMargaretAction; // Linked to the aboutMargaret() slot.
    QMenu *programsMenu; // Contains addProgramAction and deleteProgramAction.
    QMenu *helpMenu; // Contains aboutQtAction and helpMargaretAction.
    dialog_addprogram *dialogAdd; // Dialog to select programs to add.
    dialog_deleteprogram *dialogRemove; // Dialog to select programs to delete.
    Monitor *monitor; // Communicates with the OS and calculates times.
    QTableWidget *table; // Displays the data.
    int nColumns; // Stores the number of columns.
    QStringList *program; // Stores the programs to be monitored.
    QStringList *counter; // Stores the counters for each monitored program.
};

#endif // MAINWINDOW_H

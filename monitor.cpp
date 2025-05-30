#include "windowinfo.h"
#include "mainwindow.h"
#include "engine.h"

#include <QDebug>
#include <windows.h>
#include <QString>
#include <QLocale>
#include <QFile>
#include <QDate>

Monitor::Monitor(MainWindow *w) {
    mainWindow = w; // mainWindow points to the original main window.
    transformer = new QLocale; // Allocate memory for the transformer.
    maxPrograms = 50; // Will eventually be synchronized with the rest.
    nPrograms = 0; // Initialize to 0.
    waitTime = 10; // Wait time of 10 seconds.
    date = new QDate; // Allocate memory for the date.
    for (int i = 0; i < maxPrograms; i++) {
        programTime[i] = 0; // Initialize each program's time to 0.
    }
}

// Same as the above constructor, but also stores the counter map passed as a parameter.
Monitor::Monitor(MainWindow *w, const QMap<QString, std::uint64_t> &counterMap) {
    mainWindow = w;
    transformer = new QLocale;
    maxPrograms = 50;
    nPrograms = 0;
    waitTime = 10;
    date = new QDate;
    for (int i = 0; i < maxPrograms; i++) {
        programTime[i] = 0;
    }
    this->counterMap = counterMap;
}

// Method that runs concurrently with the GUI. Tracks application runtime.
void Monitor::run() {
    int day = 0;
    int previousDay = 0;

    int i = 0;
    bool emptyString = false;
    QString pr;

    QFile inputFile("programs.dat");
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Monitor: Could not open the file";
        qDebug() << qPrintable(inputFile.errorString());
        return;
    }

    QDataStream in(&inputFile);
    in.setVersion(QDataStream::Qt_6_7);
    while (!emptyString && i < maxPrograms) {
        in >> pr;
        if (pr == "") {
            emptyString = true;
        } else {
            programFile.push_back(pr);
            i++;
        }
    }
    inputFile.close();
    nPrograms = programFile.size();

    QStringList counter;
    int seconds = 0, minutes = 0, hours = 0;

    mainWindow->setProgram(programFile);

    std::vector<WindowInfo> windowInfoList;
    if (mainWindow != nullptr) {
        while (true) {
            date->currentDate().getDate(nullptr, nullptr, &day);
            if (day != previousDay) {
                *date = date->currentDate();
                mainWindow->setDate(date);
                previousDay = day;
                for (int i = 0; i < nPrograms; i++) {
                    programTime[i] = 0 + counterMap[programFile[i]];
                    counter.append("0");
                }
            }

            counterMap.clear();
            clear_windowInfoList();
            EnumWindows(EnumWindowsProc, NULL);
            windowInfoList = get_windowInfoList();

            for (int i = 0; i < nPrograms; i++) {
                if (programTime[i] < 60) {
                    counter[i] = transformer->toString(programTime[i]) + " s";
                } else if (programTime[i] < 3600) {
                    minutes = programTime[i] / 60;
                    seconds = programTime[i] % 60;
                    counter[i] = transformer->toString(minutes) + " min " + transformer->toString(seconds) + " s";
                } else {
                    hours = programTime[i] / 3600;
                    minutes = programTime[i] / 60 - hours * 60;
                    counter[i] = transformer->toString(hours) + " h " + transformer->toString(minutes) + " min ";
                }
                if (contains(windowInfoList, programFile[i].toStdString())) {
                    programTime[i] += waitTime;
                }
            }
            mainWindow->setCounter(counter);
            sleep(waitTime);
        }
    }
}

// Returns the number of programs to monitor.
int Monitor::get_nPrograms() {
    return nPrograms;
}

// Saves the counters for each program to the file counters.dat
bool Monitor::saveCounter() {
    bool saved = false; // Will be true if all counters are successfully saved.

    // The file stores the current date so Margaret only loads counters from the same day.
    int fileDay = 0;
    int fileMonth = 0;
    int fileYear = 0;

    QMap<QString, std::uint64_t> counterMap; // Maps each program to its corresponding counter.

    for (int i = 0; i < nPrograms; i++) {
        counterMap.insert(programFile[i], programTime[i]); // Store each program's counter.
    }

    // Open counters.dat for writing:
    QFile outputFile("counters.dat");
    QDataStream out(&outputFile);

    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Monitor: Could not open the file";
        qDebug() << qPrintable(outputFile.errorString()); // Print error message if opening fails.
    } else {
        out.setVersion(QDataStream::Qt_6_7); // Use Qt 6.7 format for writing.
        date->currentDate().getDate(&fileYear, &fileMonth, &fileDay); // Get current date values.
        out << fileDay << fileMonth << fileYear << counterMap; // Write date and counter map to file.
        saved = true; // Successfully saved.
        outputFile.close(); // Close the file.
    }

    return saved; // Return whether saving was successful.
}

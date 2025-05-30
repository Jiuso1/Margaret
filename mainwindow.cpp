#include "mainwindow.h"
#include "dialog_addprogram.h"
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QHBoxLayout>
#include <QWidget>
#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QMap>
#include <QFile>
#include <QMessageBox>

// Constructor
MainWindow::MainWindow()
{
    allocateMemory(); // Allocates memory for pointer attributes other than menus and actions.

    nColumns = 2; // The number of columns in the table is always 2 ("Program" and "Time").

    // Variables to store the current date and the number of programs to monitor:
    int day = 0;
    int month = 0;
    int year = 0;
    int desiredPrograms = 0;

    QTableWidgetItem *item = new QTableWidgetItem; // Allocate memory for the table item.

    QFont programFont; // Font for program names.
    programFont.setBold(true); // Set font to bold.

    QMap<QString, std::uint64_t> counterMap; // Stores the counter map read from file.

    QString dateStyle; // Stores the CSS style for the date.

    date->currentDate().getDate(&year, &month, &day); // Get current date values.

    *dateString = "<h1><center>" + transformer->toString(day) + "/" + transformer->toString(month) + "/" + transformer->toString(year) + "</center></h1>"; // Format date as string.
    dateStyle = "font: lighter;"; // Light font style.

    dateLabel->setText(*dateString); // Set date label text.
    dateLabel->setStyleSheet(dateStyle); // Apply style to date label.

    // Add dateLabel and table to the vertical layout with spacing:
    verticalLayout->addWidget(dateLabel);
    verticalLayout->addSpacing(10);
    verticalLayout->addWidget(table);

    createActions(); // Create actions.
    createMenus(); // Create menus.

    centralWidget->setLayout(verticalLayout); // Set layout for central widget.
    setCentralWidget(centralWidget); // Set central widget.
    setWindowTitle("Margaret"); // Set window title.
    setWindowIcon(QIcon("./moon.png")); // Set window icon.

    counterMap = readCounter(); // Read counter map from file.
    if (counterMap.size() == 0) {
        monitor = new Monitor(this); // No counters for today.
    } else {
        monitor = new Monitor(this, counterMap); // Pass today's counters to monitor.
    }

    monitor->start(); // Start monitor in a separate thread.

    // Table properties:
    table->showGrid(); // Show cell borders.
    table->setColumnCount(nColumns); // Set number of columns.
    desiredPrograms = monitor->get_nPrograms(); // Get number of programs to monitor.
    table->setRowCount(desiredPrograms); // Set number of rows.
    table->setSelectionMode(QAbstractItemView::NoSelection); // Disable selection.
    table->setMinimumWidth(sizeHint().width()); // Set minimum width.
    table->setMinimumHeight(sizeHint().height()); // Set minimum height.
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Stretch columns.

    // Table CSS:
    QString tableStyle = "font-size: 14pt; font-weight: 350;";
    table->setStyleSheet(tableStyle); // Apply style.

    // Initialize counters to 0:
    for (int i = 0; i < table->rowCount(); i++) {
        counter->append("0");
    }

    // Set horizontal header:
    item->setText("Program");
    table->setHorizontalHeaderItem(0, item);
    item = new QTableWidgetItem;
    item->setText("Time");
    table->setHorizontalHeaderItem(1, item);
    table->verticalHeader()->hide(); // Hide vertical header.

    // Fill program column:
    for (int i = 0; i < table->rowCount(); i++) {
        item = new QTableWidgetItem;
        item->setText(program->at(i));
        item->setFlags(Qt::ItemIsEnabled); // Make item non-editable.
        item->setFont(programFont); // Apply bold font.
        table->setItem(i, 0, item);
    }

    table->setCurrentCell(-1, -1); // Deselect all cells.
}

// SLOT to open the add program dialog.
void MainWindow::addProgram() {
    dialogAdd = new dialog_addprogram(this); // Set parent to this.
    dialogAdd->show(); // Show dialog.
}

// SLOT to open the delete program dialog.
void MainWindow::deleteProgram() {
    dialogRemove = new dialog_deleteprogram(this); // Set parent to this.
    dialogRemove->show(); // Show dialog.
}

// SLOT to show an about dialog.
void MainWindow::aboutMargaret() {
    QMessageBox::about(this, "About Margaret",
                       "<h2>Margaret</h2> <h3>Alpha Version 2 - May 2025</h3>"
                       "<p>Margaret is a free software application under the "
                       "<a href=\"https://www.gnu.org/licenses/gpl-3.0.en.html#license-text\">GPL-3.0 license</a><br>"
                       "that tracks the runtime of selected applications.</p>"
                       "<p>All source code is available on <a href=\"https://github.com/Jiuso1/Margaret\">GitHub</a>.</p>");
}

// Allocates memory for actions, assigns text, and connects SIGNAL-SLOT.
void MainWindow::createActions() {
    // Create the following actions:
    addProgramAction = new QAction("&Add program", this); // "Add Program" action with shortcut A.
    deleteProgramAction = new QAction("&Delete program", this); // "Delete Program" action with shortcut E.
    aboutQtAction = new QAction("About &Qt", this); // "About Qt" action with shortcut Q.
    helpMargaretAction = new QAction("&Margaret documentation", this); // "Margaret Documentation" action with shortcut D.
    helpMargaretAction->setVisible(false); // Hide the documentation action since it's not ready yet.
    aboutMargaretAction = new QAction("About &Margaret", this); // "About Margaret" action with shortcut M.

    // Connect actions to their respective slots:
    connect(addProgramAction, SIGNAL(triggered()), this, SLOT(addProgram()));
    connect(deleteProgramAction, SIGNAL(triggered()), this, SLOT(deleteProgram()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutMargaretAction, SIGNAL(triggered()), this, SLOT(aboutMargaret()));
}

// Creates menus, adds actions to them, and adds them to the main window's menu bar.
void MainWindow::createMenus() {
    programsMenu = menuBar()->addMenu(tr("&Programs")); // "Programs" menu with shortcut P.
    programsMenu->addAction(addProgramAction);
    programsMenu->addAction(deleteProgramAction);

    helpMenu = menuBar()->addMenu(tr("&Help")); // "Help" menu with shortcut H.
    helpMenu->addAction(helpMargaretAction);
    helpMenu->addAction(aboutMargaretAction);
    helpMenu->addAction(aboutQtAction);
}

// Updates the date in MainWindow with the one passed as a parameter.
void MainWindow::setDate(QDate *date) {
    int day = 0, month = 0, year = 0;
    date->getDate(&year, &month, &day);
    *dateString = "<h1><center>" + transformer->toString(day) + "/" + transformer->toString(month) + "/" + transformer->toString(year) + "</center></h1>";
    dateLabel->setText(*dateString);
}

// Updates the list of programs to monitor.
void MainWindow::setProgram(const QStringList &program) {
    *(this->program) = program;
}

// Updates the list of counters and refreshes the table.
void MainWindow::setCounter(const QStringList &counter) {
    *(this->counter) = counter;
    for (int i = 0; i < table->rowCount(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(counter.at(i)); // Formats: SS s | MM min SS s | HH h MM min SS s
        item->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 1, item);
    }
}

// Saves all process counters to a file.
bool MainWindow::saveCounter() {
    return monitor->saveCounter();
}

// Reads counters from file and returns a map<Program, Counter>.
QMap<QString, std::uint64_t> MainWindow::readCounter() {
    QMap<QString, std::uint64_t> counterMap;
    QDate *currentDate = new QDate;

    int fileDay = 0, fileMonth = 0, fileYear = 0;
    int currentDay = 0, currentMonth = 0, currentYear = 0;

    QFile inputFile("counters.dat");
    QDataStream in(&inputFile);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "MainWindow: Could not open the file";
        qDebug() << qPrintable(inputFile.errorString());
    } else {
        in.setVersion(QDataStream::Qt_6_7);
        in >> fileDay >> fileMonth >> fileYear;
        currentDate->currentDate().getDate(&currentYear, &currentMonth, &currentDay);
        if (fileDay == currentDay && fileMonth == currentMonth && fileYear == currentYear) {
            in >> counterMap;
        }
    }

    delete currentDate;
    return counterMap;
}

// Triggered when the application is closed. Saves the counter map to file.
void MainWindow::closeEvent(QCloseEvent *event) {
    saveCounter();
}

// Allocates memory for MainWindow's pointer attributes, excluding menus and actions.
void MainWindow::allocateMemory() {
    verticalLayout = new QVBoxLayout;
    dateLabel = new QLabel;
    date = new QDate;
    dateString = new QString;
    transformer = new QLocale;
    centralWidget = new QWidget;
    dialogAdd = nullptr; // Initialized to nullptr in case it's never opened.
    table = new QTableWidget;
    program = new QStringList;
    counter = new QStringList;
}

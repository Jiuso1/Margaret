#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // Declare the application, passing command-line arguments.
    MainWindow *window = new MainWindow; // Allocate memory for the main window.
    window->show(); // Show the window.
    return app.exec(); // Execute the application.
}

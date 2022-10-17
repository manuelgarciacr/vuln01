#include "mainwindow.h"

#include <QApplication>
#include <string>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // GUI Qt Application
    MainWindow w;               // Main window of the application


    if (argc > 1) {
        w.loadData(argv[1]);    // Loads data from the file name program argument
    }
    w.show();                   // Shows the main window

    return a.exec();            // Executes the application
}


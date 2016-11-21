#include "mainwindow.h"
#include <QApplication>
#include <canvaswidget.h>

int main(int argc, char *argv[])
{
    //QApplication manages the GUI application's control flow
    QApplication a(argc, argv);

    //create a main window
    QMainWindow qmw;

    //create a widget that spans the entire main window
    //QWidget* mainWidget = new QWidget();
    CanvasWidget* mainWidget = new CanvasWidget();
    qmw.setCentralWidget(mainWidget);
    qmw.show();

    //start the application's event loop
    return a.exec();
}

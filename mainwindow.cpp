#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::appVersion = "1.0";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle("ContourDetection v. " + appVersion);
    resize(1000, 700);


    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

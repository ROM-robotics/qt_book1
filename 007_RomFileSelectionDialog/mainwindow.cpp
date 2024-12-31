#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QString fileName = QFileDialog::getOpenFileName(this);
    QString fileName = QFileDialog::getOpenFileName(this,
             "rom dynamics",
              QDir::currentPath(),
              "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");
    qDebug() << fileName;
}

MainWindow::~MainWindow()
{
    delete ui;
}


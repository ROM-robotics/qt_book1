#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_mappingBtn_clicked()
{
    // publish mapping
}


void MainWindow::on_navigationBtn_clicked()
{
    // publish navigation
}


void MainWindow::on_remappingBtn_clicked()
{
    // publish remapping
}


void MainWindow::on_uBtn_pressed()
{
    // move forward
}


void MainWindow::on_lBtn_pressed()
{
    // move left
}


void MainWindow::on_rBtn_pressed()
{
    // move right
}


void MainWindow::on_estopBtn_clicked()
{
    // estop
}


void MainWindow::on_shutdownBtn_clicked()
{
    // shutdown
}


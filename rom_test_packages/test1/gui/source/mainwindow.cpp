#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROS2_With_Qt");
    ui = std::make_unique<Ui::MainWindow>();
    ui->setupUi(this);

    label = ui->subscribedText;
}

void MainWindow::DisplaySubscription(const QString &log)
{
    label->setText(log);
}



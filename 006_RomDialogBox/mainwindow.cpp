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


void MainWindow::on_noIconBtn_clicked()
{
    QMessageBox::about(this, "Serious Question", "Am I an awesome guy?");
}


void MainWindow::on_questionBtn_clicked()
{
    QMessageBox::question(this, "Alert", "Just to let you know, something happened!");
}


void MainWindow::on_warnBtn_clicked()
{
    QMessageBox::warning(this, "Alert", "Just to let you know, something happened!");
}


void MainWindow::on_infoBtn_clicked()
{
    QMessageBox::information(this, "Alert", "Just to let you know, something happened!");
}


void MainWindow::on_criticalBtn_clicked()
{
    QMessageBox::critical(this, "Alert", "Just to let you know, something happened!");
}


void MainWindow::on_custom1Btn_clicked()
{
    QMessageBox::question(this, "Serious Question", "Am I an awesome guy?", QMessageBox::Ignore, QMessageBox::Yes);
}


void MainWindow::on_custom2Btn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Alert");
    msgBox.setText("Just to let you know, something happened!");
    msgBox.exec();
}


void MainWindow::on_custom3Btn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Serious Question");
    msgBox.setText("Am I an awesome guy?");
    msgBox.addButton("Try Hack me!", QMessageBox::YesRole);
    msgBox.addButton("or Not", QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();
}


void MainWindow::on_custIcoBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Serious Question");
    msgBox.setText("Am I an awesome guy?");
    msgBox.addButton("Seriously Yes!", QMessageBox::YesRole);
    msgBox.addButton("Well no thanks", QMessageBox::NoRole);
    QPixmap myIcon(":/haha/images/mdyblur.png");
    msgBox.setIconPixmap(myIcon);
    msgBox.exec();
}


void MainWindow::on_click1Btn_clicked()
{
    if (QMessageBox::question(this, "Question", "Some random question. Yes or no?") == QMessageBox::Yes)
    {
       QMessageBox::warning(this, "Yes", "You have pressed Yes!");
    }
    else
    {
       QMessageBox::warning(this, "No", "You have pressed No!");
    }

}


void MainWindow::on_click2Btn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Serious Question");
    msgBox.setText("Am I an awesome guy?");
    QPushButton* yesButton = msgBox.addButton("Seriously Yes!", QMessageBox::YesRole);
    QPushButton* noButton = msgBox.addButton("Well no thanks", QMessageBox::NoRole);

    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    if (msgBox.clickedButton() == (QAbstractButton*) yesButton)
    {
       QMessageBox::warning(this, "Yes", "Oh thanks! :)");
    }
    else if (msgBox.clickedButton() == (QAbstractButton*) noButton)
    {
       QMessageBox::warning(this, "No", "Oh why... :(");
    }
}


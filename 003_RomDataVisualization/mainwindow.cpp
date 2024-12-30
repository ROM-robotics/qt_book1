#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>



#define path_to_userDB "/home/.romrobotics/rom_user.sqlite"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    animation1 = new QPropertyAnimation(ui->pushButton,"geometry");
    animation1->setDuration(1000);
    animation1->setStartValue(QRect(320, 0, 101, 101));
    animation1->setEndValue(QRect(320, 60, 101,101));
    animation1->start();

    animation2 = new QPropertyAnimation(ui->loginBtn,"geometry");
    animation2->setDuration(1000);
    animation2->setStartValue(QRect(31, 525, 221, 31));
    animation2->setEndValue(QRect(31, 273, 221, 31));

    animation3 = new QPropertyAnimation(ui->labelUserName,"geometry");
    animation3->setDuration(1000);
    animation3->setStartValue(QRect(0, 120, 65, 28));
    animation3->setEndValue(QRect(31, 120, 65, 28));

    animation4 = new QPropertyAnimation(ui->lineEditUserName,"geometry");
    animation4->setDuration(1000);
    animation4->setStartValue(QRect(700, 140, 221, 31));
    animation4->setEndValue(QRect(31, 140, 221, 31));

    animation5 = new QPropertyAnimation(ui->labelPassword,"geometry");
    animation5->setDuration(1000);
    animation5->setStartValue(QRect(0, 190, 62,28));
    animation5->setEndValue(QRect(30, 190, 62,28));

    animation6 = new QPropertyAnimation(ui->lineEditPassword,"geometry");
    animation6->setDuration(1000);
    animation6->setStartValue(QRect(700, 220, 221, 31));
    animation6->setEndValue(QRect(31, 220, 221, 31));

    animationGroup = new QParallelAnimationGroup;
    animationGroup->addAnimation(animation1);
    animationGroup->addAnimation(animation2);
    animationGroup->addAnimation(animation3);
    animationGroup->addAnimation(animation4);
    animationGroup->addAnimation(animation5);
    animationGroup->addAnimation(animation6);

    animationGroup->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loginBtn_clicked()
{
    check_password();
}


void MainWindow::check_password()
{
    QFileInfo check_users_db(path_to_userDB);
    QString input_name = ui->lineEditUserName->text();
    QString input_pass = ui->lineEditPassword->text();

    if( !( input_name =="" || input_pass == "") )
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName("0.0.0.0");
            db.setPort(3306);
            db.setDatabaseName("rom_dynamics_database");
            db.setUserName("root");
            db.setPassword("ghostman");

//            if(check_users_db.isFile())
//            {
//            //db.setDatabaseName(path_to_userDB);                         //my_sql database changed
//            db.setDatabaseName("rom_dynamics_database");


            if(!db.open())
            {
                QMessageBox::warning(this,"Failed!","E001: DB Connection မအောင်မြင်ပါ။");
            } else
            {


                QSqlQuery query("SELECT * FROM users");
                if(query.exec("SELECT name,password FROM users WHERE name =\'"+ input_name +
                          "\' AND password =\'" + input_pass +"\'"))
                {
                    if(query.next())
                    {
                        //QMessageBox::information(this,"Success!","DB login အောင်မြင်သည်။");
                        // တခြားဟာတွေဒီမှာလုပ်ပါ။
                        //ui->setupUi(user_interface.ui);
                        on_adminbtn_clicked();
                    }
                    else
                    {
                        QMessageBox::warning(this,"Error!","E002: အသုံးပြုသူအမည် သို့မဟုတ် လျှို့ဝှက်ကုတ် မှားနေသည်။");

                    }
                }
                else
                {
                    QMessageBox::warning(this,"Error!","E003: users Table မရှိသေးပါ။");
                }
            }
        }
//        else
//        {
//            QMessageBox::warning(this,"Not Existed!","E004: DB ဖိုင်မရှိပါ။");
//        }

//    }
//    else
//    {
//        QMessageBox::warning(this,"Blank Name!","E001: အမည်နှင့် လျှို့ဝှက်နံပါတ်ထည့်ပါ။");
//    }
    clear_and_focus();
}

void MainWindow::clear_and_focus(){
    ui->lineEditUserName->clear();
    ui->lineEditPassword->clear();
    ui->lineEditUserName->setFocus();
}

void MainWindow::on_lineEditUserName_editingFinished()
{
    ui->lineEditPassword->setFocus();
}

void MainWindow::on_lineEditPassword_editingFinished()
{
    ui->centralwidget->setFocus();
}

void MainWindow::on_adminbtn_clicked()
{
    this->hide();
    adminDialog adm_dialog;
    adm_dialog.setModal(true);
    adm_dialog.exec();

}

//void MainWindow::on_clientbtn_clicked()
//{
//    //this->hide();
//    clientDialog cli_dialog;
//    cli_dialog.setModal(true);
//    cli_dialog.exec();
//}

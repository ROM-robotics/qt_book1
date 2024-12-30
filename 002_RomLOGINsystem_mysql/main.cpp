#include "mainwindow.h"

#include <QApplication>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();


//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//        db.setHostName("0.0.0.0");
//        db.setPort(3306);
//        db.setDatabaseName("rom_dynamics_database");
//        db.setUserName("root");
//        db.setPassword("ghostman");

//        if (db.open())
//        {
//            qDebug() << "Connected! in main.cpp" ;
//        }
//        else
//        {
//            qDebug() << "Failed to connect db: in main.cpp" << db.lastError().text();
//            return 0;
//        }


//        QSqlQuery query("SELECT * FROM users");

//            if (!query.exec()) {
//                qDebug() << "Error executing query:" << query.lastError().text();
//                return 1;
//            }

//            while (query.next()) {
//                int id = query.value(0).toInt();  // Get the value of the first column
//                QString name = query.value("name").toString();  // Get the value of 'name' column

//                qDebug() << "ID:" << id << "Name:" << name;
//            }

        //QStringList drivers = QSqlDatabase::drivers();
        //qDebug() << "Available drivers:" << drivers;
    return a.exec();
}

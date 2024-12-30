#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QFileInfo>
#include <QSqlQuery>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <admindialog.h>
#include <clientdialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clear_and_focus();
private slots:
    void on_loginBtn_clicked();

    void on_lineEditUserName_editingFinished();

    void on_lineEditPassword_editingFinished();



    void on_adminbtn_clicked();

    void on_clientbtn_clicked();

private:
    Ui::MainWindow *ui;
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;
    QPropertyAnimation *animation3;
    QPropertyAnimation *animation4;
    QPropertyAnimation *animation5;
    QPropertyAnimation *animation6;
    QParallelAnimationGroup *animationGroup;
    void check_password();

};
#endif // MAINWINDOW_H

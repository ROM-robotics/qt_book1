#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "rclcpp/rclcpp.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mappingBtn_clicked();

    void on_navigationBtn_clicked();

    void on_remappingBtn_clicked();

    void on_uBtn_pressed();

    void on_lBtn_pressed();

    void on_rBtn_pressed();

    void on_estopBtn_clicked();

    void on_shutdownBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

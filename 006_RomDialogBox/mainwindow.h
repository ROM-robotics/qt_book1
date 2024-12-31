#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

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
    void on_noIconBtn_clicked();

    void on_questionBtn_clicked();

    void on_warnBtn_clicked();

    void on_infoBtn_clicked();

    void on_criticalBtn_clicked();

    void on_custom1Btn_clicked();

    void on_custom2Btn_clicked();

    void on_custom3Btn_clicked();

    void on_custIcoBtn_clicked();

    void on_click1Btn_clicked();

    void on_click2Btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QFileInfo>
#include <QSqlQuery>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QtCharts>
#include <QChartView>

#include <QBarSet>
#include <QBarSeries>

#include <QPieSeries>
#include <QPieSlice>

#include <QLineSeries>

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
    void check_password_database();
    bool check_password_fake();

protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void on_stackedWidget_currentChanged(int arg1);

    void on_loginBtn_clicked();

    void on_lineEditUserName_editingFinished();

    void on_lineEditPassword_editingFinished();

private:
    Ui::MainWindow *ui;
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;
    QPropertyAnimation *animation3;
    QPropertyAnimation *animation4;
    QPropertyAnimation *animation5;
    QPropertyAnimation *animation6;
    QParallelAnimationGroup *animationGroup;

    QChartView *chartViewBar;
    QChartView *chartViewPie;
    QChartView *chartViewLine;

};
#endif // MAINWINDOW_H

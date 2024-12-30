#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

/* to do => to add sqlite file to project's resource */
//#define path_to_userDB "/home/.romrobotics/rom_user.sqlite"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    animation1 = new QPropertyAnimation(ui->pushButton,"geometry");
//    animation1->setDuration(1000);
//    animation1->setStartValue(QRect(320, 0, 101, 101));
//    animation1->setEndValue(QRect(320, 60, 101,101));
//    animation1->start();

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
    //animationGroup->addAnimation(animation1);
    animationGroup->addAnimation(animation2);
    animationGroup->addAnimation(animation3);
    animationGroup->addAnimation(animation4);
    animationGroup->addAnimation(animation5);
    animationGroup->addAnimation(animation6);

    animationGroup->start();

    ////////BAR CHART/////////////
       /* performance အတွက် data များနှင့် object များခွဲထားရန် */
       QBarSet *set0 = new QBarSet("Jane");
       QBarSet *set1 = new QBarSet("John");
       QBarSet *set2 = new QBarSet("Axel");
       QBarSet *set3 = new QBarSet("Mary");
       QBarSet *set4 = new QBarSet("Samantha");

       *set0 << 10 << 20 << 30 << 40 << 50 << 60;
       *set1 << 50 << 70 << 40 << 45 << 80 << 70;
       *set2 << 30 << 50 << 80 << 13 << 80 << 50;
       *set3 << 50 << 60 << 70 << 30 << 40 << 25;
       *set4 << 90 << 70 << 50 << 30 << 16 << 42;

       QBarSeries *seriesBar = new QBarSeries();
          seriesBar->append(set0);
          seriesBar->append(set1);
          seriesBar->append(set2);
          seriesBar->append(set3);
          seriesBar->append(set4);

          QChart *chartBar = new QChart();
          chartBar->addSeries(seriesBar);
          chartBar->setTitle("Students Performance");
          chartBar->setAnimationOptions(QChart::SeriesAnimations);

          QStringList categories;
          categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
          QBarCategoryAxis *axis = new QBarCategoryAxis();
          axis->append(categories);
          chartBar->createDefaultAxes();
          chartBar->setAxisX(axis, seriesBar);

          chartViewBar = new QChartView(chartBar);
          chartViewBar->setRenderHint(QPainter::Antialiasing);
          chartViewBar->setParent(ui->chart1);

          QPieSeries *seriesPie = new QPieSeries();
          seriesPie->append("Jane", 10);
          seriesPie->append("Joe", 20);
          seriesPie->append("Andy", 30);
          seriesPie->append("Barbara", 40);
          seriesPie->append("Jason", 50);

          QPieSlice *slice = seriesPie->slices().at(1);
          slice->setExploded();
          slice->setLabelVisible();
          slice->setPen(QPen(Qt::darkGreen, 2));
          slice->setBrush(Qt::green);

          QChart *chartPie = new QChart();
          chartPie->addSeries(seriesPie);
          chartPie->setTitle("Students Performance");

          chartViewPie = new QChartView(chartPie);
          chartViewPie->setRenderHint(QPainter::Antialiasing);
          chartViewPie->setParent(ui->chart2);

          QLineSeries *seriesLine = new QLineSeries();
          seriesLine->append(0, 6);
          seriesLine->append(2, 4);
          seriesLine->append(3, 8);
          seriesLine->append(7, 4);
          seriesLine->append(10, 5);
          seriesLine->append(11, 10);
          seriesLine->append(13, 3);
          seriesLine->append(17, 6);
          seriesLine->append(18, 3);
          seriesLine->append(20, 2);

          QChart *chartLine = new QChart();
          chartLine->addSeries(seriesLine);
          chartLine->createDefaultAxes();
          chartLine->setTitle("Students Performance");

          chartViewLine = new QChartView(chartLine);
          chartViewLine->setRenderHint(QPainter::Antialiasing);
          chartViewLine->setParent(ui->chart3);

}


bool MainWindow::check_password_fake()
{
    //qDebug() << "check_password_fake()";
    return true;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   chartViewBar->resize(chartViewBar->parentWidget()->size());
   chartViewPie->resize(chartViewPie->parentWidget()->size());
   chartViewLine->resize(chartViewLine->parentWidget()->size());

}


void MainWindow::check_password_database(){}


void MainWindow::clear_and_focus(){
    ui->lineEditUserName->clear();
    ui->lineEditPassword->clear();
    ui->lineEditUserName->setFocus();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1 == 1)
    {
        // do something with data for charts
    }
}


void MainWindow::on_loginBtn_clicked()
{
    //qDebug() << "on_loginBtn_clicked()";
    //check_password_database();
    if(check_password_fake()== true)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void MainWindow::on_lineEditUserName_editingFinished()
{
    ui->lineEditPassword->setFocus();
}


void MainWindow::on_lineEditPassword_editingFinished()
{
    //ui->centralwidget->setFocus();
    ui->loginBtn->setFocus();
}


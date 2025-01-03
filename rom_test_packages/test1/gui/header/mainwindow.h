
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QLabel>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
    
    public slots:
        void DisplaySubscription(const QString &log);
    
    private:
        std::unique_ptr<Ui::MainWindow> ui = nullptr;
        QLabel* label = nullptr;
};

#endif // MAINWINDOW_H

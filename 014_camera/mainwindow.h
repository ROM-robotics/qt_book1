#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectCamera();

private slots:
    void on_connectButton_clicked();
    void cameraError(QCamera::Error error);

private:
    Ui::MainWindow *ui;

    QCamera* camera;
    QCameraViewfinder* viewfinder;
    bool connected;

};
#endif // MAINWINDOW_H

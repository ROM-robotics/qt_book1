#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connected = false;
    camera = new QCamera();

    qDebug() << "Number of cameras found:" << QCameraInfo::availableCameras().count();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        qDebug() << "Camera info:" << cameraInfo.deviceName() <<
        cameraInfo.description() << cameraInfo.position();

        ui->deviceSelection->addItem(cameraInfo.description());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    if (!connected)
       {
             connectCamera();
       }
       else
       {
             camera->stop();
             viewfinder->deleteLater();
             ui->connectButton->setText("Connect");
             connected = false;
       }
}


void MainWindow::connectCamera()
{
   QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
   foreach (const QCameraInfo &cameraInfo, cameras)
   {
         qDebug() << cameraInfo.description() << ui->deviceSelection->currentText();

         if (cameraInfo.description() == ui->deviceSelection->currentText())
         {
               camera = new QCamera(cameraInfo);
               viewfinder = new QCameraViewfinder(this);
               camera->setViewfinder(viewfinder);
               ui->webcamLayout->addWidget(viewfinder);

               connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(cameraError(QCamera::Error)));

               connected = true;
               ui->connectButton->setText("Disconnect");

               camera->start();

               return;
         }
   }
}


void MainWindow::cameraError(QCamera::Error error)
{
   qDebug() << "Camera error:" << error;

   connected = false;
   camera->stop();
   ui->connectButton->setText("Connect");
}

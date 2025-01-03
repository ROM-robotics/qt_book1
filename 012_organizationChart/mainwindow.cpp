#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

    profileBox* box = new profileBox();
    box->init("John Doe", this, scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectionChanged()
{
    qDebug() << "Item selected";
}


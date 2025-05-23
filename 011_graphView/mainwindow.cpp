#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(scene,
            &QGraphicsScene::selectionChanged,
            this,
            &MainWindow::selectionChanged);

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen pen(Qt::black);
    pen.setWidth(2);

    /// Example 1
    /*
    QGraphicsRectItem* rectangle = scene->addRect(80, 0, 80, 80, pen, greenBrush);
    QGraphicsEllipseItem* ellipse = scene->addEllipse(0, -80, 200, 60, pen, blueBrush);
    QGraphicsTextItem* text = scene->addText("Hello World!", QFont("Times", 25));
    */
    /// Example 2
    QGraphicsRectItem* rectangle = scene->addRect(80, 0, 80, 80, pen, greenBrush);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);
    rectangle->setFlag(QGraphicsItem::ItemIsSelectable);

    QGraphicsEllipseItem* ellipse = scene->addEllipse(0, -80, 200, 60, pen, blueBrush);
    ellipse->setFlag(QGraphicsItem::ItemIsMovable);
    ellipse->setFlag(QGraphicsItem::ItemIsSelectable);

    QGraphicsTextItem* text = scene->addText("Hello World!", QFont("Times", 25));
    text->setFlag(QGraphicsItem::ItemIsMovable);
    text->setFlag(QGraphicsItem::ItemIsSelectable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectionChanged()
{
   qDebug() << "Item selected";
}

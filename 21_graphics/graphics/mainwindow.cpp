#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    // Set up the custom GraphicsView
    GraphicsView *view = new GraphicsView(this);
    view->setScene(scene);
    setCentralWidget(view);
    ui->graphicsView->
}

MainWindow::~MainWindow()
{
    delete ui;
}



//-------------------

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent), scene_(nullptr), drawing(false), currentLine_(nullptr) {}

void GraphicsView::setScene(QGraphicsScene *scene)
{
    scene_ = scene;
    QGraphicsView::setScene(scene_);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (!scene_) return;

    QPointF scenePos = mapToScene(event->pos());

    if (event->button() == Qt::RightButton)
    {
        drawing = true;
        startPoint_ = scenePos;

        currentLine_ = scene_->addLine(QLineF(startPoint_, startPoint_),
                                       QPen(Qt::red, 2, Qt::DotLine));
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing && (event->buttons() & Qt::RightButton) && currentLine_)
    {
        QPointF scenePos = mapToScene(event->pos());
        currentLine_->setLine(QLineF(startPoint_, scenePos));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && drawing)
    {
        drawing = false;
        currentLine_ = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image = QImage(this->size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    // Temporary layer for waypoints များ
    wayPointImage_ = QImage(this->size(), QImage::Format_ARGB32);
    //wayPointImage_.fill(Qt::transparent); 
    wayPointImage_.fill(Qt::red); 


    drawing = false;
    brushColor = Qt::black;
    brushSize = 4;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 1. right click ဆိုရင် drawing line ဆွဲမှာ
    if (event->button() == Qt::RightButton) 
    {
        drawing = true;
        obsStartPoint_ = event->pos();  

    }

    // 2. left click ဆိုရင် way point ထည့်မှာ/ ဖျက်မှာ
    else if (event->button() == Qt::LeftButton) 
    {
        
        
        // waypoint ထည့်ရန်။
        
        //QPainter painter(&image);
        //wayPointImage_.fill(Qt::transparent);

        QPainter painter(&wayPointImage_);

        QPen pen(Qt::blue);           // Set the pen color to blue
        pen.setWidth(2);              // Thin pen for border
        painter.setPen(pen);

        QBrush brush(Qt::green);     // Fill the waypoint with yellow
        painter.setBrush(brush);
            
        int radius = 10;  
        QRect ellipseRect(event->pos() - QPoint(radius, radius), QSize(radius * 2, radius * 2));

        waypointEllipses_.append(ellipseRect);
        waypointPositions_.append(event->pos());

        // clear ပြန်လုပ်တာဖြစ်တယ်။
        wayPointImage_.fill(Qt::transparent);

        for (int i = 0; i < waypointEllipses_.size(); ++i)
        {  
            const QRect &ellipse = waypointEllipses_.at(i); // Access by reference
            painter.drawEllipse(ellipse); 
            qDebug() << QString("Created ellipse from ထ  ည့်  ရ   န်။ %1").arg(i);
        }

        
        
        this->update();
        
    }
    else if (event->button() == Qt::MiddleButton) 
    {
        //  waypoint ဖျက်ရန်။   
        qDebug()<<"left double clicked ";
        QPoint clickedPoint = event->pos();
            /////////
        QPainter painter(&wayPointImage_);

        QPen pen(Qt::blue);           // Set the pen color to blue
        pen.setWidth(2);              // Thin pen for border
        painter.setPen(pen);

        QBrush brush(Qt::green);     // Fill the waypoint with yellow
        painter.setBrush(brush);

        
        int removalRadius = 15; 

        // Iterate over waypoints to find the one to remove
        for (int i = 0; i < waypointPositions_.size(); ++i)
        {
            if ((waypointPositions_[i] - clickedPoint).manhattanLength() <= removalRadius)
            {
                waypointPositions_.removeAt(i);
                waypointEllipses_.removeAt(i);

                // clear ပြန်လုပ်တာဖြစ်တယ်။
                wayPointImage_.fill(Qt::transparent);

                for (int i = 0; i < waypointEllipses_.size(); ++i)
                {  
                    const QRect &ellipse = waypointEllipses_.at(i); // Access by reference
                    painter.drawEllipse(ellipse); 
                    qDebug() << QString("Created ellipse from ထု တ် ရ  န်  %1").arg(i);
                }

                this->update();
                return; // Exit after removing one waypoint
            }
        }
        
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::RightButton) && drawing)
    {
        obsLastPoint_ = event->pos();
        this->update();
    }
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        drawing = false;
        obsLastPoint_ = event->pos();

        QPainter painter(&image);

        QPen pen(Qt::red);           // Set the color to red
        pen.setStyle(Qt::DotLine);   // Set the line style to dotted
        pen.setWidth(brushSize);     // Set the width (size of dots)
        pen.setCapStyle(Qt::RoundCap); // Optional: Make dots rounded

        painter.setPen(pen);  // Apply the pen

        // Draw a straight line from startPoint to endPoint
        painter.drawLine(obsStartPoint_, obsLastPoint_);

        // Trigger a repaint to display the updated image
        this->update();
    }
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter canvasPainter(this);
    canvasPainter.drawImage(this->rect(), image, image.rect());
    canvasPainter.drawImage(this->rect(), wayPointImage_, wayPointImage_.rect());

    if (drawing)
    {
        QPen pen(Qt::red);           // Set the color to red
        pen.setStyle(Qt::DotLine);   // Set the line style to dotted
        pen.setWidth(brushSize);     // Set the width (size of dots)
        pen.setCapStyle(Qt::RoundCap); // Optional: Make dots rounded

        canvasPainter.setPen(pen);

        // Draw the moving line (not saved to the image)
        canvasPainter.drawLine(obsStartPoint_, obsLastPoint_);
    }
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QImage newImage(event->size(), QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), image);
    image = newImage;

    QImage newTempImage(event->size(), QImage::Format_ARGB32);
    wayPointImage_.fill(Qt::transparent);

    QPainter painter2(&newTempImage);
    painter.drawImage(QPoint(0, 0), wayPointImage_);
    wayPointImage_ = newTempImage;
}




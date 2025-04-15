#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath> // For atan2 and M_PI

extern void shutdown_thread();

#define ROM_Q_DEBUG 1
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROM Dynamics Company's Robot Suite");
    ui = std::make_shared<Ui::MainWindow>();
    ui->setupUi(this);

    zoom_btn_ptr_ = ui->zoomBtn;
    waypoints_btn_ptr_ = ui->addWaypointBtn;
    virtual_wall_btn_ptr_ = ui->addWallBtn;
    eraser_btn_ptr_ = ui->eraserBtn;
    normal_btn_ptr_ = ui->normalBtn;

    qRegisterMetaType<nav_msgs::msg::OccupancyGrid::SharedPtr>("nav_msgs::msg::OccupancyGrid::SharedPtr");

    connect(zoom_btn_ptr_, &QPushButton::clicked, this, &MainWindow::onZoomButtonClicked);
    connect(waypoints_btn_ptr_, &QPushButton::clicked, this, &MainWindow::onWayPointsButtonClicked);
    connect(virtual_wall_btn_ptr_, &QPushButton::clicked, this, &MainWindow::onWallButtonClicked);
    connect(eraser_btn_ptr_, &QPushButton::clicked, this, &MainWindow::onEraserButtonClicked);
    connect(normal_btn_ptr_, &QPushButton::clicked, this, &MainWindow::onNormalButtonClicked);

}


void MainWindow::mousePressEvent(QMouseEvent *event) {

    if(!checkGraphicViewAndScene()) {ui->statusLabel->setText("checkGraphicViewAndScene() not ok."); return; }

    // Get the position of the click relative to the QGraphicsView
    QPoint viewPoint = ui->graphicsView->mapFrom(this, event->pos());

    // Map the QGraphicsView coordinates to the QGraphicsScene coordinates
    QPointF scenePoint = ui->graphicsView->mapToScene(viewPoint);

    // Check if the scenePoint is within the scene's bounds
    QRectF sceneBounds = ui->graphicsView->scene()->sceneRect();

    if (!sceneBounds.contains(scenePoint)) 
    {
        // If the point is outside the scene, ignore it
        ui->statusLabel->setText("Click outside scene bounds ignored.");
        return;
    }

    else 
    {   // if the point inside the scene, do your work
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        // 1. zoom mode
        if(zoom_mode_)
        {
            if (event->button() == Qt::LeftButton) 
            {
                ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            }
            QMainWindow::mousePressEvent(event);

            /* mouse scroll */
            setDragMode(true);

            return;
        }

        // 2. waypoints mode 
        else if(waypoints_mode_)
        {   
            setDragMode(false);

            ui->statusLabel->setText("Click inside scene bounds.");

            // Convert the scene coordinates to map coordinates
            double mapX = (scenePoint.x() * this->map_resolution_) + this->map_origin_x_;
            double mapY = (scenePoint.y() * this->map_resolution_) + this->map_origin_y_;
            double yaw  = 0.00001; // sample

            /********************** add points to points list ***************************/
            if (event->button() == Qt::LeftButton) 
            {
                int radius = 10;
                QRectF circle(scenePoint.x() - radius, scenePoint.y() - radius, radius * 3, radius * 3);
                
                QGraphicsEllipseItem* circleItem = new QGraphicsEllipseItem(circle);
                // Set properties
                circleItem->setPen(QPen(Qt::green, 2));  
                circleItem->setBrush(QBrush(Qt::green)); 

                // create Dialog Box-----------------------------------------------
                bool is_name_ok;
                bool is_dir_ok;
                QString wp_name; double wp_heading;

                wp_name = QInputDialog::getText(this, tr("Waypoint"), tr("Table name :"), QLineEdit::Normal, "", &is_name_ok);
                if (is_name_ok && !wp_name.isEmpty()) 
                {
                    for(int i = 0; i < waypoints_text_.size(); i++)
                    {
                        if(waypoints_text_[i]->toPlainText() == wp_name)
                        {
                            QMessageBox::information(this, tr("Cancelled"), tr("Waypoint name already exists."));
                            return;
                        }
                    }
                    wp_heading = QInputDialog::getDouble(this, tr("Enter Heading Direction"), tr("Heading (degrees):"), 0, -360, 360, 2, &is_dir_ok);
                    if (!is_dir_ok) 
                    {
                        QMessageBox::information(this, tr("Cancelled"), tr("Heading input was cancelled."));
                        return;
                    }
                } 
                else
                {
                    return;
                }
                // end Dialog Box----------------------------------------------------

                QGraphicsTextItem *textItem = ui->graphicsView->scene()->addText(wp_name);
                    textItem->setFont(QFont("Arial", 16));  // Set font and size
                    textItem->setDefaultTextColor(QColor("#067832"));  // Set text color
                    textItem->setPos(scenePoint.x()-50, scenePoint.y()-65); 

                waypoints_.append(circleItem);
                waypoints_text_.append(textItem);
                waypoints_direction_.append(wp_heading);

                // Add the rectangle to the scene
                ui->graphicsView->scene()->addItem(circleItem);
            }

            /********************* remove points to points list ***************************/
            else if (event->button() == Qt::RightButton) 
            {
                if (!waypoints_.isEmpty()) 
                {
                    QGraphicsEllipseItem *tmpItem = waypoints_.takeLast(); // Get the last item and remove from list
                    ui->graphicsView->scene()->removeItem(tmpItem);
                    delete tmpItem;  // Free memory

                    QGraphicsTextItem *text = waypoints_text_.takeLast();
                    delete text; 

                    waypoints_direction_.removeLast();

                    /************* emit data to transmit **************/
                    // virtual_lines_ ကို transmit လုပ်ပေးရမယ်။
                }
            }

            /*********** emit data to transmit **************/
            // point ထည့် / ဖြုတ်ပြီးတိုင်း romWaypoints_ ကို transmit လုပ်ပေးရမယ်။

            // Update the status label with the calculated position
            ui->statusLabel->setText(QString::asprintf("map x: %.5f, y: %.5f, yaw: %d", mapX, mapY, 0));
            return;
        }   

        // 3. Create Virtual Wall mode 
        else if (virtual_wall_mode_) 
        {
            if (event->button() == Qt::LeftButton) 
            {
                if(!drawing)
                {
                    drawing = true;
                    obstacleStartPoint_event_pos = new QPoint(scenePoint.x(), scenePoint.y());

                    // ---------------reddot-----------------
                    int radius = 5;
                    QRectF redDot1(scenePoint.x() - radius, scenePoint.y() - radius, radius * 3, radius * 3);
                    QGraphicsRectItem* redDot1Item = new QGraphicsRectItem(redDot1);
                    redDot1Item->setPen(QPen(Qt::red, 2));  
                    redDot1Item->setBrush(QBrush(Qt::red)); 
                    ui->graphicsView->scene()->addItem(redDot1Item);

                    virtual_lines_points_.append(redDot1Item);
                }
                else
                {
                    // ---------------reddot-----------------
                    int radius = 5;
                    QRectF redDot2(scenePoint.x() - radius, scenePoint.y() - radius, radius * 3, radius * 3);
                    QGraphicsRectItem* redDot2Item = new QGraphicsRectItem(redDot2);
                    redDot2Item->setPen(QPen(Qt::red, 2));  
                    redDot2Item->setBrush(QBrush(Qt::red)); 
                    ui->graphicsView->scene()->addItem(redDot2Item);
                    
                    obstacleLastPoint_event_pos = new QPoint(scenePoint.x(), scenePoint.y());
                    QGraphicsLineItem *line = new QGraphicsLineItem( QLine(*obstacleStartPoint_event_pos, *obstacleLastPoint_event_pos) );
                    QPen pen(Qt::red, 10, Qt::DashLine); 
                    line->setPen(pen);
                    ui->graphicsView->scene()->addItem(line);

                    // Store in the QList
                    virtual_lines_.append(line);  
                    virtual_lines_points_.append(redDot2Item);

                    drawing = false;

                    /************* emit data to transmit **************/
                    // virtual_lines_ ကို transmit လုပ်ပေးရမယ်။
                }
                #ifdef ROM_Q_DEBUG
                    //qDebug() << "obstacleStartPoint_event_pos : " << obstacleStartPoint_event_pos;
                #endif
                return;
            }
             /********************* remove line to lines list ***************************/
            else if (event->button() == Qt::RightButton) 
            {
                if (!virtual_lines_.isEmpty()) 
                {
                    QGraphicsLineItem *line = virtual_lines_.takeLast(); // Get the last item and remove from list
                    ui->graphicsView->scene()->removeItem(line);
                    delete line;  // Free memory

                    QGraphicsRectItem *redDot2 = virtual_lines_points_.takeLast();
                    delete redDot2;  

                    QGraphicsRectItem *redDot1 = virtual_lines_points_.takeLast();
                    delete redDot1;  

                    /************* emit data to transmit **************/
                    // virtual_lines_ ကို transmit လုပ်ပေးရမယ်။
                }
            }
        }

        // 4. Delete landmark mode 
        else if(eraser_mode_)
        {
            setDragMode(false);

            /********************* remove points to points list ***************************/
            if (event->button() == Qt::LeftButton) 
            {
                for (QGraphicsItem* item : ui->graphicsView->scene()->items()) 
                {
                    if (dynamic_cast<QGraphicsLineItem*>(item)) 
                    {  // Check if it's a line
                        ui->graphicsView->scene()->removeItem(item);
                        delete item;
                    }
                    else if (dynamic_cast<QGraphicsEllipseItem*>(item)) 
                    {  // Check if it's a Rect Point
                        ui->graphicsView->scene()->removeItem(item);
                        delete item;
                    }
                    else if (dynamic_cast<QGraphicsTextItem*>(item)) 
                    {  // Check if it's a Rect Point
                        ui->graphicsView->scene()->removeItem(item);
                        delete item;
                    }
                    else if (dynamic_cast<QGraphicsRectItem*>(item)) 
                    {  // Check if it's a Rect Point
                        ui->graphicsView->scene()->removeItem(item);
                        delete item;
                    }
                }
                // clear lists
                virtual_lines_.clear();
                virtual_lines_points_.clear();

                waypoints_.clear();
                waypoints_text_.clear();
                waypoints_direction_.clear();
            }

            /************* emit data to transmit **************/
            // romWaypoints_ ကို transmit လုပ်ပေးရမယ်။
            // virtual_lines_ ကို transmit လုပ်ပေးရမယ်။

            return;
        }

        // 5. Default mode
        else if(normal_mode_)
        {
            setDragMode(false);
            
        }
    }
}


/*************  ✨ Codeium Command ⭐  *************/
/**
 * @brief Updates the map image based on the given OccupancyGrid message.
 *
 * This function takes the given OccupancyGrid message and updates the map image
 * in the GraphicsView. The map image is converted from the OccupancyGrid data
 * and then set as the scene of the GraphicsView.
 */
/******  d8a4a59e-027e-4f6a-a099-a001cadf608e  *******/
void MainWindow::onUpdateMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) 
{
    qDebug() << "onUpdateMap slot function";
    double map_origin_x   = msg->info.origin.position.x;
    double map_origin_y   = msg->info.origin.position.y;
    double map_resolution = msg->info.resolution;

    // Convert OccupancyGrid to an image
    QImage mapImage(msg->info.width, msg->info.height, QImage::Format_RGB888);
    for (size_t y = 0; y < msg->info.height; ++y) {
        for (size_t x = 0; x < msg->info.width; ++x) {
            int index = y * msg->info.width + x;
            int value = msg->data[index];
            QColor color = (value == 0) ? Qt::white : (value == 100) ? Qt::black : Qt::gray;
            mapImage.setPixel(x, y, color.rgb());
        }
    }
    this->map_resolution_ = map_resolution;
    this->map_origin_x_ = map_origin_x;
    this->map_origin_y_ = map_origin_y;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(mapImage));
    ui->graphicsView->setScene(scene);
    
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    qDebug() << "onUpdateMap finished";
}

bool MainWindow::checkGraphicViewAndScene()
{
    if (!ui->graphicsView) {
        ui->statusLabel->setText("GraphicsView is null.");
        return false;
    }

    if (!ui->graphicsView->scene()) {
        ui->statusLabel->setText("GraphicsView has no scene."); 
        return false;
    }
    else {
        return true;
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) 
{
    if(zoom_mode_)
    {
        if (ui->graphicsView->scene()) 
        {
            constexpr double scaleFactor = 1.2;  // Zoom factor

            if (event->angleDelta().y() > 0) {
                ui->graphicsView->scale(scaleFactor, scaleFactor);  // Zoom in
            } else {
                ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // Zoom out
            }
        }
    }
    
}

void MainWindow::setDragMode(bool state)
{
    if(state)
    {
        /* mouse scroll */
        //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        /* if not zoom_mode */
        //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }
}

void MainWindow::onZoomButtonClicked()
{
    #ifdef ROM_Q_DEBUG
        qDebug() << "zoom btn clicked";
    #endif
    ui->statusLabel->setText("Zoom Button Clicked.");
    zoom_mode_ = true;
    waypoints_mode_   = false;
    virtual_wall_mode_ = false;
    eraser_mode_ = false;
    normal_mode_ = false;

    ui->zoomBtn->setStyleSheet("color: white; background-color: none; border: 2px solid white;");
    ui->addWaypointBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWallBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->eraserBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->normalBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
}
void MainWindow::onWayPointsButtonClicked()
{
    #ifdef ROM_Q_DEBUG
        qDebug() << "waypoint btn clicked";
    #endif
    ui->statusLabel->setText("Waypoints Button Clicked.");
    zoom_mode_ = false;
    waypoints_mode_   = true;
    virtual_wall_mode_ = false;
    eraser_mode_ = false;
    normal_mode_ = false;

    ui->zoomBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWaypointBtn->setStyleSheet("color: white; background-color: none; border: 2px solid white;");
    ui->addWallBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->eraserBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->normalBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
}
void MainWindow::onWallButtonClicked()
{
    #ifdef ROM_Q_DEBUG
        qDebug() << "wall btn clicked";
    #endif
    ui->statusLabel->setText("Wall Button Clicked.");
    zoom_mode_ = false;
    waypoints_mode_   = false;
    virtual_wall_mode_ = true;
    eraser_mode_ = false;
    normal_mode_ = false;

    ui->zoomBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWaypointBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWallBtn->setStyleSheet("color: white; background-color: none; border: 2px solid white;");
    ui->eraserBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->normalBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
}
void MainWindow::onEraserButtonClicked()
{
    #ifdef ROM_Q_DEBUG
        qDebug() << "Eraser btn clicked";
    #endif
    ui->statusLabel->setText("Eraser Button Clicked.");
    zoom_mode_ = false;
    waypoints_mode_   = false;
    virtual_wall_mode_ = false;
    eraser_mode_ = true;
    normal_mode_ = false;

    ui->zoomBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWaypointBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWallBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->eraserBtn->setStyleSheet("color: white; background-color: none; border: 2px solid white;");
    ui->normalBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
}
void MainWindow::onNormalButtonClicked()
{
    #ifdef ROM_Q_DEBUG
        qDebug() << "Normal btn clicked";
    #endif
    ui->statusLabel->setText("Normal Button Clicked.");
    zoom_mode_ = false;
    waypoints_mode_   = false;
    virtual_wall_mode_ = false;
    eraser_mode_ = false;
    normal_mode_ = true;

    ui->zoomBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWaypointBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->addWallBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->eraserBtn->setStyleSheet("color: #979ba1; background-color: none; border: 2px solid gray;");
    ui->normalBtn->setStyleSheet("color: white; background-color: none; border: 2px solid white;");
}


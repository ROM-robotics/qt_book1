#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath> // For atan2 and M_PI

//QPointF initialScenePoint; // Store the initial point
auto clicked_pose = geometry_msgs::msg::PoseStamped();

extern void shutdown_thread();

#define ROM_Q_DEBUG 1
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROM Dynamics Company's Robot Suite");
    ui = std::make_shared<Ui::MainWindow>();
    ui->setupUi(this);

    clicked_pose.header.frame_id = "map";

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
    if (!sceneBounds.contains(scenePoint)) {
        // If the point is outside the scene, ignore it
        ui->statusLabel->setText("Click outside scene bounds ignored.");
        return;
    }
    else 
    {   // if the point inside the scene, do your work
        if(zoom_mode_)
        {
            if (event->button() == Qt::LeftButton) 
            {
                ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            }
            QMainWindow::mousePressEvent(event);

            /* mouse scroll */
            setScrollDragMode(true);

            return;
        }
        else if(waypoints_mode_)
        {   
            setScrollDragMode(false);

            ui->statusLabel->setText("Click inside scene bounds.");

            // Convert the scene coordinates to map coordinates
            double mapX = (scenePoint.x() * this->map_resolution_) + this->map_origin_x_;
            double mapY = (scenePoint.y() * this->map_resolution_) + this->map_origin_y_;

            // Publish the pose
            // clicked_pose.header.stamp = rclcpp::Clock().now();
            // clicked_pose.pose.position.x = mapX;
            // clicked_pose.pose.position.y = mapY;
            // clicked_pose.pose.orientation.w = 1.0; // Neutral orientation
            // pose_publisher_->publish(clicked_pose);

            // Update the status label with the calculated position
            ui->statusLabel->setText(QString::asprintf("x: %.5f, y: %.5f, phi: %d", mapX, mapY, 0));
            return;
        }   
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
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if(zoom_mode_)
    {
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        QMainWindow::mouseReleaseEvent(event);
    }
}


void MainWindow::onUpdateMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) 
{
    qDebug() << "onUpdateMap updated";
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

void MainWindow::setScrollDragMode(bool state)
{
    if(state)
    {
        /* mouse scroll */
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        /* if not zoom_mode */
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }
}

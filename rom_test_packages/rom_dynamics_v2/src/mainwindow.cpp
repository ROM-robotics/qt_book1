#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROM Dynamics Company's Robot Suite");
    ui = std::make_shared<Ui::MainWindow>();
    ui->setupUi(this);

    //label = ui->subscribedText; // to delete

    node_ = rclcpp::Node::make_shared("rom_qt");
    mode_publisher_ = node_->create_publisher<std_msgs::msg::String>("switch_mode", 10);
    pose_publisher_ = node_->create_publisher<geometry_msgs::msg::PoseStamped>("pose", 10);

    connect(ui->mappingBtn, &QPushButton::clicked, this, &MainWindow::sendMappingMode);
    connect(ui->navigationBtn, &QPushButton::clicked, this, &MainWindow::sendNavigationMode);
    connect(ui->remappingBtn, &QPushButton::clicked, this, &MainWindow::sendRemappingMode);

    ui->shutdownBtn->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"        // Background color when pressed
    "}"
    );
    ui->btnEstop->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"        // Background color when pressed
    "}"
    );
    ui->btnStop->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"        // Background color when pressed
    "}"
    );

    ui->btnForward->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"        // Background color when pressed
    "}"
    );
    ui->btnLeft->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"        // Background color when pressed
    "}"
    );
    ui->btnRight->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"        // Background color when pressed
    "}"
    );

    current_mode_ = "navi";
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF clickedPoint = ui->graphicsView->mapFromGlobal(event->globalPos());
    double mapX = (clickedPoint.x() * this->map_resolution_) + this->map_origin_x_;
    double mapY = (clickedPoint.y() * this->map_resolution_) + this->map_origin_y_;

    // Publish the pose
    auto pose = geometry_msgs::msg::PoseStamped();
    pose.header.frame_id = "map";
    pose.header.stamp = rclcpp::Clock().now();
    pose.pose.position.x = mapX;
    pose.pose.position.y = mapY;
    pose.pose.orientation.w = 1.0; // Neutral orientation
    pose_publisher_->publish(pose);
    ui->statusLabel->setText(QString::asprintf("x: %.5f, y: %.5f, phi: %d", mapX, mapY, 0));

}


void MainWindow::updateMap(const QImage& mapImage, const double map_origin_x, const double map_origin_y, const double map_resolution) 
{
    this->map_resolution_ = map_resolution;
    this->map_origin_x_ = map_origin_x;
    this->map_origin_y_ = map_origin_y;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(mapImage));
    ui->graphicsView->setScene(scene);
    
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::DisplaySubscription(const QString &log)
{
    //ui->statusLabel->setText("hello"); there is no publisher
}


void MainWindow::sendMappingMode() {

    if( current_mode_ == "mapping" ) {
        return;
    } else 
    {
        current_mode_ = "mapping";

        auto msg = std_msgs::msg::String();
        msg.data = "mapping";

        mode_publisher_->publish(msg);
        ui->statusLabel->setText("Changing Mapping Mode...");
        ui->mappingBtn->setStyleSheet("background-color: green; font-weight: bold;");
        ui->navigationBtn->setStyleSheet("background-color: none; font-weight: bold;");
        ui->remappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
    }
}


void MainWindow::sendNavigationMode() {
    if( current_mode_ == "navi" ) {
        return;
    } else 
    {
        current_mode_ = "navi";

        auto msg = std_msgs::msg::String();
        msg.data = "navi";

        mode_publisher_->publish(msg);
        ui->statusLabel->setText("Changing Navigation Mode...");
        ui->mappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
        ui->navigationBtn->setStyleSheet("background-color: green; font-weight: bold;");
        ui->remappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
    }
}


void MainWindow::sendRemappingMode() {
    if(current_mode_ == "remapping") {
        return;
    } else 
    {
        current_mode_ = "remapping";
    
        auto msg = std_msgs::msg::String();
        msg.data = "remapping";

        mode_publisher_->publish(msg);
        ui->statusLabel->setText("Changing Remapping Mode...");
        ui->mappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
        ui->navigationBtn->setStyleSheet("background-color: none; font-weight: bold;");
        ui->remappingBtn->setStyleSheet("background-color: green; font-weight: bold;");
    }
}


void MainWindow::on_shutdownBtn_clicked()
{
  ui->statusLabel->setText("Shutdown 0 ...");
}


void MainWindow::on_btnEstop_clicked()
{
  ui->statusLabel->setText("Activating E-Stop ...");
}

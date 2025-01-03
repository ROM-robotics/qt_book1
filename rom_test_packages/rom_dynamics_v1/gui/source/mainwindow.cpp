#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROM Dynamics Company's Robot Suite");
    ui = std::make_shared<Ui::MainWindow>();
    ui->setupUi(this);

    label = ui->subscribedText; // to delete

    node_ = rclcpp::Node::make_shared("rom_qt");
    mode_publisher_ = node_->create_publisher<std_msgs::msg::String>("switch_mode", 10);

    connect(ui->mappingBtn, &QPushButton::clicked, this, &MainWindow::sendMappingMode);
    connect(ui->navigationBtn, &QPushButton::clicked, this, &MainWindow::sendNavigationMode);
    connect(ui->remappingBtn, &QPushButton::clicked, this, &MainWindow::sendRemappingMode);

    current_mode_ = "navi";
}

void MainWindow::updateMap(const QImage& mapImage) 
{
    label->setPixmap(QPixmap::fromImage(mapImage));
}

void MainWindow::DisplaySubscription(const QString &log)
{
    //label->setText(log);
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

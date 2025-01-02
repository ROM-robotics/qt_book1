#include "main_window.hpp"
#include "ui_main_window.h"

#include <std_srvs/srv/empty.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

QtRosPublisher::QtRosPublisher(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
{
    ui->setupUi(this);

    rclcpp::init(0, nullptr);
    node_ = std::make_shared<rclcpp::Node>("qt_ros_publisher");
    publisher_ = node_->create_publisher<std_msgs::msg::String>("chatter", 10);

    // Timer to simulate periodic publishing
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &QtRosPublisher::publishMessage);
    timer_->start(1000); // 1 Hz
}

QtRosPublisher::~QtRosPublisher() {
    delete ui;
}


void QtRosPublisher::publishMessage()
{
        auto msg = std::make_shared<std_msgs::msg::String>();
        msg->data = "Hello from ROS 2!";
        publisher_->publish(*msg);
        RCLCPP_INFO(node_->get_logger(), "Publishing: '%s'", msg->data.c_str());

        ui->label->setText(QString::fromStdString(msg->data)); // Update GUI label
}


void QtRosPublisher::runRosSpin()
{
    rclcpp::spin(node_);
}
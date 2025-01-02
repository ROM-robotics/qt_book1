#include "main_window.hpp"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
    {
        ui->setupUi(this);

        // Set up ROS 2
        //rclcpp::init(0, nullptr);
        //node_ = std::make_shared<rclcpp::Node>("qt_ros_publisher_threaded");
        node_ = rclcpp::Node::make_shared("qt_ros2_thread");

        // Create a publisher for the "chatter" topic
        publisher_ = node_->create_publisher<std_msgs::msg::String>("chatter", 10);

        // Set up a multi-threaded executor for handling ROS 2 callbacks
        executor_ = std::make_shared<rclcpp::executors::MultiThreadedExecutor>(rclcpp::ExecutorOptions(), 4);
        //executor_ = std::make_shared<rclcpp::executors::MultiThreadedExecutor>(rclcpp::executors::MultiThreadedExecutor(), 4); // 4 threads
        executor_->add_node(node_);

        // Start a thread to run the ROS 2 executor
        ros_thread_ = new QThread(this);
        connect(ros_thread_, &QThread::started, this, &MainWindow::runRosExecutor);
        ros_thread_->start();

        // Timer to periodically publish messages
        timer_ = new QTimer(this);
        connect(timer_, &QTimer::timeout, this, &MainWindow::publishMessage);
        timer_->start(1000);  // 1 Hz
    }

MainWindow::~MainWindow()
    {
        rclcpp::shutdown();
        ros_thread_->quit();
        ros_thread_->wait();
    }


void MainWindow::publishMessage()
    {
        count_ += 1;
        auto msg = std::make_shared<std_msgs::msg::String>();
        msg->data = "QT ROS 2 with Multi-threaded Executor! count = " + std::to_string(count_);
        publisher_->publish(*msg);
        RCLCPP_INFO(node_->get_logger(), "Publishing: '%s'", msg->data.c_str());

        // Update the Qt label with the last message
        ui->label->setText(QString::fromStdString(msg->data));
    }

void MainWindow::runRosExecutor()
    {
        // Run the ROS 2 executor in the background thread
        executor_->spin();
    }
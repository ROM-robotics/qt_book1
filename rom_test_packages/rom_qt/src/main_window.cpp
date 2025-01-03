#include "main_window.hpp"
#include "ui_main_window.h"

#include <std_srvs/srv/empty.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
{
    ui->setupUi(this);

    node_ = rclcpp::Node::make_shared("rom_qt");
    /*
    // Setup ROS 2 executor
        executor_thread_ = std::thread([this]() { rclcpp::spin(node_); });

    // Create RViz rendering panel
        render_panel_ = new rviz_common::RenderPanel(this);

    // Set up visualization manager
        vis_manager_ = new rviz_common::VisualizationManager(&render_panel_);
        render_panel_->initialize(vis_manager_->getSceneManager(), vis_manager_);

     // Initialize the visualization manager
        vis_manager_->initialize();
        vis_manager_->startUpdate();

        // Add Map Display
        map_display_ = vis_manager_->createDisplay("rviz_default_plugins/Map", "Map", true);
        map_display_->subProp("Topic")->setValue("/map");
    
    // Add TF Display
        tf_display_ = vis_manager_->createDisplay("rviz_default_plugins/TF", "TF", true);

        // Set up the layout
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(render_panel_);
        setLayout(layout);
        //ui->myVerticalLayout->addWidget(render_panel_);
        //setLayout(ui->myVerticalLayout);
    */
    mode_publisher_ = node_->create_publisher<std_msgs::msg::String>("switch_mode", 10);
    twist_publisher_ = node_->create_publisher<geometry_msgs::msg::Twist>("cmd_vel_web_to_twist", 10);
    // goal_publisher_ = node_->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);

    // map_saver_client_ = node_->create_client<std_srvs::srv::Empty>("map_saver");

    //connect(ui->saveMapButton, &QPushButton::clicked, this, &MainWindow::saveMap);
    //connect(ui->navigateButton, &QPushButton::clicked, this, &MainWindow::navigateToGoal);
    connect(ui->mappingBtn, &QPushButton::clicked, this, &MainWindow::sendMappingMode);
    connect(ui->navigationBtn, &QPushButton::clicked, this, &MainWindow::sendNavigationMode);
    connect(ui->remappingBtn, &QPushButton::clicked, this, &MainWindow::sendRemappingMode);

    connect(ui->btnForward, &QPushButton::clicked, this, &MainWindow::drive_forward);
    connect(ui->btnLeft, &QPushButton::clicked, this, &MainWindow::drive_left);
    connect(ui->btnRight, &QPushButton::clicked, this, &MainWindow::drive_right);
    connect(ui->btnEstop, &QPushButton::clicked, this, &MainWindow::drive_stop);
}

MainWindow::~MainWindow() {
    delete ui;
    /*
    if (executor_thread_.joinable())
    {
        rclcpp::shutdown();
        executor_thread_.join();
    }
    */
}

void MainWindow::sendMappingMode() {
    auto msg = std_msgs::msg::String();
    msg.data = "mapping";

    mode_publisher_->publish(msg);
    ui->statusLabel->setText("Changing Mapping Mode...");
    ui->mappingBtn->setStyleSheet("background-color: green; font-weight: bold;");
    ui->navigationBtn->setStyleSheet("background-color: none; font-weight: bold;");
    ui->remappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
}
void MainWindow::sendNavigationMode() {
    auto msg = std_msgs::msg::String();
    msg.data = "navi";

    mode_publisher_->publish(msg);
    ui->statusLabel->setText("Changing Navigation Mode...");
    ui->mappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
    ui->navigationBtn->setStyleSheet("background-color: green; font-weight: bold;");
    ui->remappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
}
void MainWindow::sendRemappingMode() {
    auto msg = std_msgs::msg::String();
    msg.data = "remapping";

    mode_publisher_->publish(msg);
    ui->statusLabel->setText("Changing Remapping Mode...");
    ui->mappingBtn->setStyleSheet("background-color: none; font-weight: bold;");
    ui->navigationBtn->setStyleSheet("background-color: none; font-weight: bold;");
    ui->remappingBtn->setStyleSheet("background-color: green; font-weight: bold;");
}

void MainWindow::drive_forward()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 0.5;
    msg.angular.z = 0.0;
    twist_publisher_->publish(msg);
}
void MainWindow::drive_left()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 0.0;
    msg.angular.z = -0.5;
    twist_publisher_->publish(msg);
}
void MainWindow::drive_right()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 0.0;
    msg.angular.z = 0.5;
    twist_publisher_->publish(msg);
}
void MainWindow::drive_stop()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 0.0;
    msg.angular.z = 0.0;
    twist_publisher_->publish(msg);
}
// void MainWindow::saveMap() {
//     if (!map_saver_client_->wait_for_service(std::chrono::seconds(5))) {
//         ui->statusLabel->setText("Map Saver service not available!");
//         return;
//     }

//     auto request = std::make_shared<std_srvs::srv::Empty::Request>();
//     map_saver_client_->async_send_request(request);
//     ui->statusLabel->setText("Map saved successfully.");
// }

// void MainWindow::navigateToGoal() {
//     auto goal = geometry_msgs::msg::PoseStamped();
//     goal.header.frame_id = "map";
//     goal.header.stamp = node_->get_clock()->now();
//     goal.pose.position.x = ui->xInput->text().toDouble();
//     goal.pose.position.y = ui->yInput->text().toDouble();
//     goal.pose.orientation.w = 1.0;

//     goal_publisher_->publish(goal);
//     ui->statusLabel->setText("Navigating to goal...");
// }

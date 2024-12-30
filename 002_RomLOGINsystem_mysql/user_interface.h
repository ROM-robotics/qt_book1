#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <QMainWindow>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/empty.hpp>

#include <rviz_common/render_panel.hpp>
#include <rviz_common/visualization_manager.hpp>
#include <rviz_default_plugins/displays/map/map_display.hpp>
#include <rviz_default_plugins/displays/tf/tf_display.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void saveMap();
    //void navigateToGoal();
    void sendMappingMode();
    void sendNavigationMode();
    void sendRemappingMode();

    void driveForward();
    void driveRight();
    void driveLeft();

    void eStop();
    void shutdownPC();

private:
    Ui::MainWindow *ui;
    rclcpp::Node::SharedPtr node_;

    /*
    std::thread executor_thread_;
    rviz_common::RenderPanel *render_panel_;
    rviz_common::VisualizationManager *vis_manager_;
    rviz_common::Display *map_display_;
    rviz_common::Display *tf_display_;
    */
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_publisher_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mode_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr goal_publisher_;

    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr map_saver_client_;
};



#endif // USER_INTERFACE_H

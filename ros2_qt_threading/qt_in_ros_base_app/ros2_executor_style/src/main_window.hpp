#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QThread>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executor.hpp>

#include <std_msgs/msg/string.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void publishMessage();

    void runRosExecutor();

private:
    Ui::MainWindow *ui;
    QTimer *timer_;
    QThread *ros_thread_;

    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor_;
    int count_ = 0;
};

#endif // MAIN_WINDOW_HPP
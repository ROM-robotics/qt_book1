#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QApplication>
#include <QMainWindow>
#include <QTimer>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QtRosPublisher : public QMainWindow
{
    Q_OBJECT

public:
    QtRosPublisher(QWidget *parent = nullptr);
    ~QtRosPublisher();

public slots:
    void publishMessage();

    void runRosSpin();

private:
    Ui::MainWindow *ui;
    QTimer *timer_;
    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

#endif // MAIN_WINDOW_HPP


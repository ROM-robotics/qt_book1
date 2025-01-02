#include "main_window.hpp"
#include <rclcpp/rclcpp.hpp>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtRosPublisher window;
    window.show();

    // ROS spin in main thread (combined with Qt)
    window.runRosSpin();

    return app.exec();
}

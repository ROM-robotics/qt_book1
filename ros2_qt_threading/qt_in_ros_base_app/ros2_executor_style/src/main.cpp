
#include "main_window.hpp"
#include <rclcpp/rclcpp.hpp>
#include <QApplication>
//#include <rclcpp/executor.hpp>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    QApplication app(argc, argv);

    MainWindow widget;
    widget.show();

    return app.exec();
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QLabel>

#include "ui_mainwindow.h"

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        //~MainWindow() { delete ui;}
        std::shared_ptr<Ui::MainWindow> getUi() { return ui; }
    
    public slots:
        void DisplaySubscription(const QString &log);
        void updateMap(const QImage& mapImage);
        
        void sendMappingMode();
        void sendNavigationMode();
        void sendRemappingMode();
    
    private:
        std::shared_ptr<Ui::MainWindow> ui = nullptr;
        QLabel* label = nullptr;

        rclcpp::Node::SharedPtr node_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mode_publisher_;
        std::string current_mode_;
};

#endif // MAINWINDOW_H

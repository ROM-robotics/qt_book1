
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "ui_mainwindow.h"

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/string.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>

#include <example_interfaces/srv/add_two_ints.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class ServiceClient : public QObject {
    Q_OBJECT

public:
    explicit ServiceClient();
    ~ServiceClient();

    void sendRequest(int a, int b);

signals:
    void responseReceived(int sum);

private:
    rclcpp::Node::SharedPtr node;
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client;
    std::thread rosThread;

    void spin();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        std::shared_ptr<Ui::MainWindow> getUi() { return ui; }
    
    public slots:
        void DisplaySubscription(const QString &log);
        
        void sendMappingMode();
        void sendNavigationMode();
        void sendRemappingMode();

        // void saveMapClicked();
        // void openMapClicked();
        // void selectMapClicked();
        
    private slots:
        void on_shutdownBtn_clicked();
        void on_btnEstop_clicked();

    private slots:
        //void onSendRequest();
        void onResponseReceived(int sum);  
    
    private:
        std::shared_ptr<Ui::MainWindow> ui = nullptr;
        
        QPushButton *sendMappingBtnPtr_;
        QPushButton *sendNavigationBtnPtr_;
        QPushButton *sendRemappingBtnPtr_;

        std::string current_mode_;
        // QPushButton *saveMapBtnPtr_;
        // QPushButton *openMapBtnPtr_;
        // QPushButton *selectMapBtn;

        QLabel* statusLabelPtr_ = nullptr;

        ServiceClient *service_client_;
        QThread *rosThread;
};

#endif // MAINWINDOW_H

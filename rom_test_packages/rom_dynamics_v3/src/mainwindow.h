
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QLabel>

#include "ui_mainwindow.h"

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/string.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>

// header 
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

        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        
        bool checkGraphicViewAndScene();
        void setScrollDragMode(bool state);
    
    public slots:
        void onUpdateMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
        void onZoomButtonClicked();
        void onWayPointsButtonClicked();
        void onWallButtonClicked();
        void onEraserButtonClicked();
        void onNormalButtonClicked();
        
    private slots:
        
    
    private:
        std::shared_ptr<Ui::MainWindow> ui = nullptr;
        QLabel* label = nullptr;

        std::string current_mode_;

        // map
        double map_origin_x_ = 0;
        double map_origin_y_ = 0;
        double map_resolution_ = 0;

        // 5 modes
        bool zoom_mode_ = false;
        bool waypoints_mode_   = false;
        bool virtual_wall_mode_ = false;
        bool eraser_mode_ = false;
        bool normal_mode_ = true;

        QPushButton *zoom_btn_ptr_;
        QPushButton *waypoints_btn_ptr_;
        QPushButton *virtual_wall_btn_ptr_;
        QPushButton *eraser_btn_ptr_;
        QPushButton *normal_btn_ptr_;
};

#endif // MAINWINDOW_H

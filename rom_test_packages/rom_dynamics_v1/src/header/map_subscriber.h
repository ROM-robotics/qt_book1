#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QObject>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <QImage>

class MapSubscriber : public QObject, public rclcpp::Node {
    Q_OBJECT

public:
    explicit MapSubscriber(QObject* parent = nullptr);

signals:
    void mapUpdated(const QImage& mapImage); // Signal to notify about map updates

private:
    void mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);

    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_subscription_;
};

#endif // MAPHANDLER_H

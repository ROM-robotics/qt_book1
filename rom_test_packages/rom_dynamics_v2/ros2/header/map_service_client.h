#ifndef MAP_CLIENT_H
#define MAP_CLIENT_H
#include "rclcpp/rclcpp.hpp"
#include "rom_interfaces/srv/which_maps.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <sstream>
#include <QObject>
#include <QtWidgets/QApplication>

using namespace std::chrono_literals;

class MapServiceClient : public QObject, public rclcpp::Node
{
    Q_OBJECT
public:
    explicit MapServiceClient(const std::string &service_name);

public slots:
    // Method to call the service and print the result
    void callServiceOpen();
    void callServiceSave();
    void callServiceSelect();

private:
    std::shared_ptr<rom_interfaces::srv::WhichMaps::Request> request_;
    rclcpp::Client<rom_interfaces::srv::WhichMaps>::SharedPtr client_;
};
#endif


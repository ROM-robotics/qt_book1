#include "rclcpp/rclcpp.hpp"
#include "rom_interfaces/srv/which_maps.hpp"
#include "map_service_client.h"
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <sstream>

using namespace std::chrono_literals;

MapServiceClient::MapServiceClient(const std::string &service_name) : Node("map_service_client")
{
        // Create the service client
        client_ = this->create_client<rom_interfaces::srv::WhichMaps>(service_name);

       
        // while (!client_->wait_for_service(1s)) {
        //     if (!rclcpp::ok()) {
        //         //RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for service. Exiting.");
        //         return;
        //     }
        //     //RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
        // }
}

void MapServiceClient::callServiceOpen()
{
    // Create the request
    //auto request = std::make_shared<rom_interfaces::srv::WhichMaps::Request>();
        
    request_->map_request = "which_maps";
    request_->requested_map_name = "default_map";
        
    auto result = client_->async_send_request(request_);
    // if (rclcpp::spin_until_future_complete(this, result) == rclcpp::FutureReturnCode::SUCCESS) {
        
    // } else 
    // {
           
           
    // }
}

void MapServiceClient::callServiceSave()
{
    // Create the request
    auto request = std::make_shared<rom_interfaces::srv::WhichMaps::Request>();
        
    request_->map_request = "zzz";
    request_->requested_map_name = "";
        
    auto result = client_->async_send_request(request_);
    // if (rclcpp::spin_until_future_complete(this, result) == rclcpp::FutureReturnCode::SUCCESS) {
        
    // } else 
    // {
        
    // }
}

void MapServiceClient::callServiceSelect()
{
    // Create the request
    auto request = std::make_shared<rom_interfaces::srv::WhichMaps::Request>();
        
    request_->map_request = "zz";
    request_->requested_map_name = "";
        
    auto result = client_->async_send_request(request_);
    
    // if (rclcpp::spin_until_future_complete(this, result) == rclcpp::FutureReturnCode::SUCCESS) {
            
    // } else 
    // {
        
    // }
}


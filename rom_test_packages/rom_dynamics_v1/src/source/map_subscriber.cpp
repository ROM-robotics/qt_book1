#include <map_subscriber.h>

MapSubscriber::MapSubscriber(QObject* parent) : QObject(parent), Node("map_handler") 
{
    map_subscription_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/map", 10, std::bind(&MapSubscriber::mapCallback, this, std::placeholders::_1));
}


void MapSubscriber::mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) 
{
    int width = msg->info.width;
    int height = msg->info.height;
    QImage mapImage(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            int index = y * width + x;
            int value = msg->data[index];

            int gray = 127; // Default for unknown (-1)
            if (value >= 0 && value <= 100) 
            {
                gray = static_cast<int>(255 - (value * 2.55)); // Map [0, 100] to [255, 0]
            }
            mapImage.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }
    emit mapUpdated(mapImage); // Emit updated map as QImage
}

   
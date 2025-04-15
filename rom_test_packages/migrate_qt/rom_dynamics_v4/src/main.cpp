#include "mainwindow.h"
#include "map_subscriber.h"

#include <QtWidgets/QApplication>
#include <QObject>
#include <QScreen>

// std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor = nullptr;
// std::thread executor_thread;

int main(int argc, char *argv[])
{
    setvbuf(stdout, nullptr, _IONBF, BUFSIZ); // stdout IO NO buffer
    rclcpp::init(argc, argv);

    std::shared_ptr<MapSubscriber> map_subscriber = nullptr;
    std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> map_executor_st = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();

    map_subscriber = std::make_shared<MapSubscriber>("/map");

    map_executor_st->add_node(map_subscriber);
    

    QApplication a(argc, argv);

    
    // executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    // auto map_subscriber = std::make_shared<MapSubscriber>();

    // executor->add_node(map_subscriber);
    
    a.setStyleSheet(
        "QPushButton {"
        "    border: 3px solid #8f8f91;"
        "    border-radius: 25px;" 
        "     font-weight: bold;"
        "}"
        "QLabel {"
        "    border-radius: 25px;" 
        "}"
    );

    MainWindow mainWindow;

    mainWindow.show();
    std::thread action_executor_thread([map_executor_st](){map_executor_st->spin();});
    //executor_thread = std::thread([executor]() { executor->spin(); });
    
    QObject::connect(map_subscriber.get(), &MapSubscriber::updateMap, &mainWindow, &MainWindow::onUpdateMap);

    return a.exec();
}

// void shutdown_thread()
// {
//     // Stop the ROS executor thread safely
//     if (executor) {
//         executor->cancel(); // Signal the executor to stop
//     }

//     if (executor_thread.joinable()) {
//         executor_thread.join(); // Wait for the thread to finish
//     }

//     rclcpp::shutdown();
// }
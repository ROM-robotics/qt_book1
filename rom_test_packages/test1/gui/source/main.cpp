#include "mainwindow.h"

#include "publisher.h"
#include "subscriber.h"

#include <QtWidgets/QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    setvbuf(stdout, nullptr, _IONBF, BUFSIZ); // stdout IO NO buffer
    rclcpp::init(argc, argv);

    std::shared_ptr<Publisher> publisher = nullptr;
    std::shared_ptr<Subscriber> subscriber = nullptr;
    std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor = nullptr;

    executor = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();

    publisher = std::make_shared<Publisher>("chatter");

    subscriber = std::make_shared<Subscriber>("chatter");

    executor->add_node(publisher);
    executor->add_node(subscriber);

    std::thread executor_thread([executor](){executor->spin();});

    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    QObject::connect(subscriber.get(), &Subscriber::logReceived, &mainWindow, &MainWindow::DisplaySubscription);

    return a.exec();
}

// subscriber object ကနေ signal emit လုပ်ပြီး MainWindow object ရဲ့ slot ကို ဆက်သွယ်ထားတယ်။
// subscriber က shared_ptr , အဲ့တာကိုမှ .get() နဲ့ raw pointer of မူလ Subscriber Object ကို ဆွဲထုတ်တာ။
// logReceived က Subscriber ဆီက signal ပါ။
// နောက် လက်ခံမဲ့ object ရယ်, သူ့ရဲ့ slot function ရယ်ပေါ့။

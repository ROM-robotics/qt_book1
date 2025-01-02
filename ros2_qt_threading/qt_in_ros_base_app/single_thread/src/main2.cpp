#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class QtRosPublisher : public QWidget
{
    Q_OBJECT

public:
    QtRosPublisher()
    {
        // Set up Qt GUI
        QVBoxLayout *layout = new QVBoxLayout(this);
        label_ = new QLabel("No message yet", this);
        layout->addWidget(label_);

        // Set up ROS 2
        rclcpp::init(0, nullptr);
        node_ = std::make_shared<rclcpp::Node>("qt_ros_publisher");
        publisher_ = node_->create_publisher<std_msgs::msg::String>("chatter", 10);

        // Timer to simulate periodic publishing
        timer_ = new QTimer(this);
        connect(timer_, &QTimer::timeout, this, &QtRosPublisher::publishMessage);
        timer_->start(1000); // 1 Hz
    }

    ~QtRosPublisher()
    {
        rclcpp::shutdown();
    }

public slots:
    void publishMessage()
    {
        auto msg = std::make_shared<std_msgs::msg::String>();
        msg->data = "Hello from ROS 2!";
        publisher_->publish(*msg);
        RCLCPP_INFO(node_->get_logger(), "Publishing: '%s'", msg->data.c_str());
        label_->setText(QString::fromStdString(msg->data)); // Update GUI label
    }

    void runRosSpin()
    {
        rclcpp::spin(node_);
    }

private:
    QLabel *label_;
    QTimer *timer_;
    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtRosPublisher widget;
    widget.show();

    // ROS spin in main thread (combined with Qt)
    widget.runRosSpin();

    return app.exec();
}

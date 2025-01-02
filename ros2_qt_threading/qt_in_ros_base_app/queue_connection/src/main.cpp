#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class RosPublisherThread : public QThread
{
    Q_OBJECT

public:
    RosPublisherThread() : QThread() {}
    ~RosPublisherThread() {
        if (ros_node_) {
            //ros_node_->shutdown();
        }
    }

protected:
    void run() override {
        rclcpp::init(0, nullptr);
        ros_node_ = std::make_shared<rclcpp::Node>("qt_ros_publisher_thread");
        auto publisher = ros_node_->create_publisher<std_msgs::msg::String>("chatter", 10);

        rclcpp::Rate rate(1); // 1 Hz

        while (rclcpp::ok()) {
            auto msg = std::make_shared<std_msgs::msg::String>();
            msg->data = "Hello from ROS 2 with Qt::QueuedConnection!";
            publisher->publish(*msg);
            RCLCPP_INFO(ros_node_->get_logger(), "Publishing: '%s'", msg->data.c_str());
            emit messagePublished(QString::fromStdString(msg->data)); // Emit signal to update UI
            rate.sleep();
        }
    }

signals:
    void messagePublished(const QString &msg);

private:
    rclcpp::Node::SharedPtr ros_node_;
};

class QtRosPublisher : public QWidget
{
    Q_OBJECT

public:
    QtRosPublisher() {
        // Set up Qt GUI
        QVBoxLayout *layout = new QVBoxLayout(this);
        label_ = new QLabel("No message yet", this);
        layout->addWidget(label_);
        setLayout(layout);

        // Start the ROS publisher thread
        ros_thread_ = new RosPublisherThread();
        connect(ros_thread_, &RosPublisherThread::messagePublished, this, &QtRosPublisher::updateLabel, Qt::QueuedConnection);
        ros_thread_->start();
    }

    ~QtRosPublisher() {
        ros_thread_->quit();
        ros_thread_->wait();
    }

public slots:
    void updateLabel(const QString &msg) {
        label_->setText(msg); // Update label with new message
    }

private:
    QLabel *label_;
    RosPublisherThread *ros_thread_;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtRosPublisher widget;
    widget.show();

    return app.exec();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath> // For atan2 and M_PI

extern void shutdown_thread();

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ROM Dynamics Company's Robot Suite");
    ui = std::make_shared<Ui::MainWindow>();
    ui->setupUi(this);

    statusLabelPtr_ = ui->statusLabel;
    statusLabelPtr_->setWordWrap(true); // Enable word wrapping
    statusLabelPtr_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    sendMappingBtnPtr_ = ui->mappingBtn;
    sendNavigationBtnPtr_ = ui->navigationBtn;
    sendRemappingBtnPtr_ = ui->remappingBtn;

    // Initialize the ROS Worker
    service_client_ = new ServiceClient();
    rosThread = new QThread();
    service_client_->moveToThread(rosThread);

    connect(sendMappingBtnPtr_, &QPushButton::clicked, this, &MainWindow::sendMappingMode);
    connect(sendNavigationBtnPtr_, &QPushButton::clicked, this, &MainWindow::sendNavigationMode);
    connect(sendRemappingBtnPtr_, &QPushButton::clicked, this, &MainWindow::sendRemappingMode);

    rosThread->start();

    ui->shutdownBtn->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"        // Background color when pressed
    "}"
    );
    ui->btnEstop->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"       
    "}"
    );
    ui->btnStop->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(255, 200, 200);"       
    "}"
    );

    ui->btnForward->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"      
    "}"
    );
    ui->btnLeft->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"       
    "}"
    );
    ui->btnRight->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"        
    "}"
    );

    ui->saveMapBtn->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);" 
    "}"       
    );
    ui->openMapBtn->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"       
    "}"
    );
    ui->selectMapBtn->setStyleSheet(
    "QPushButton:pressed {"
    "    background-color: rgb(200, 255, 200);"       
    "}"
    );

    current_mode_ = "navi";
    
}


MainWindow::~MainWindow()
{
    rclcpp::shutdown();

    rosThread->quit();
    rosThread->wait();

    delete service_client_;
    delete rosThread;
}



void MainWindow::DisplaySubscription(const QString &log)
{
    //statusLabelPtr_->setText("hello"); there is no publisher
}


void MainWindow::sendMappingMode() {

    if( current_mode_ == "mapping" ) 
    {
        return;
    } else 
    {
        current_mode_ = "mapping";

        int a = 99; //inputA->text().toInt();
        int b = 1;  //inputB->text().toInt();
        QMetaObject::invokeMethod(service_client_, [a, b, this]() { service_client_->sendRequest(a, b); });
        
        // msg.data = "mapping";

        statusLabelPtr_->setText("Changing Mapping Mode...");
        ui->mappingBtn->setStyleSheet("background-color: green;");
        ui->navigationBtn->setStyleSheet("background-color: none;");
        ui->remappingBtn->setStyleSheet("background-color: none;");
    }
}


void MainWindow::sendNavigationMode() {
    if( current_mode_ == "navi" ) {
        return;
    } else 
    {
        current_mode_ = "navi";

        int a = 99; //inputA->text().toInt();
        int b = 1;  //inputB->text().toInt();
        QMetaObject::invokeMethod(service_client_, [a, b, this]() { service_client_->sendRequest(a, b); });

        // msg.data = "navi";

        statusLabelPtr_->setText("Changing Navigation Mode...");
        ui->mappingBtn->setStyleSheet("background-color: none;");
        ui->navigationBtn->setStyleSheet("background-color: green;");
        ui->remappingBtn->setStyleSheet("background-color: none;");
    }
}


void MainWindow::sendRemappingMode() {
    if(current_mode_ == "remapping") {
        return;
    } else 
    {
        current_mode_ = "remapping";
    
        int a = 99; //inputA->text().toInt();
        int b = 1;  //inputB->text().toInt();
        QMetaObject::invokeMethod(service_client_, [a, b, this]() { service_client_->sendRequest(a, b); });

        // msg.data = "remapping";
        
        statusLabelPtr_->setText("Changing Remapping Mode...");
        ui->mappingBtn->setStyleSheet("background-color: none;");
        ui->navigationBtn->setStyleSheet("background-color: none;");
        ui->remappingBtn->setStyleSheet("background-color: green;");
    }
}


void MainWindow::on_shutdownBtn_clicked()
{
    statusLabelPtr_->setText("Shutdown 0 ...");

    rclcpp::shutdown(); // Stop the ROS 2 node and executor

    QApplication::quit(); // Exit the application
}


void MainWindow::on_btnEstop_clicked()
{
  statusLabelPtr_->setText("Activating E-Stop ...");
}


void MainWindow::onResponseReceived(int sum) {
    if (sum == -1) {
        statusLabelPtr_->setText("Error: Service not available or failed.");
    } else {
        statusLabelPtr_->setText(QString("Result: %1").arg(sum));
    }
}



ServiceClient::ServiceClient() {
    rclcpp::init(0, nullptr);
    node = rclcpp::Node::make_shared("qt_service_client");
    client = node->create_client<example_interfaces::srv::AddTwoInts>("/add_two_ints");

    // Start a separate thread for the ROS 2 spinning
    rosThread = std::thread(&ServiceClient::spin, this);
}

/// SERVICE CLIENT
ServiceClient::~ServiceClient() {
    rclcpp::shutdown();
    if (rosThread.joinable()) {
        rosThread.join();
    }
}

void ServiceClient::spin() {
    rclcpp::spin(node);
}

void ServiceClient::sendRequest(int a, int b) {
    auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    request->a = a;
    request->b = b;

    if (!client->wait_for_service(std::chrono::seconds(5))) {
        emit responseReceived(-1); // Error: service not available
        return;
    }

    auto future = client->async_send_request(request);
    future.wait();

    try {
        auto response = future.get();
        emit responseReceived(response->sum);
    } catch (const std::exception &e) {
        emit responseReceived(-1); // Error: response failure
    }
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QTableWidget>
#include <QToolBar>
#include <QTreeView>
#include <QWidgetAction>
#include <opencv2/imgproc/imgproc.hpp>
#include <QToolButton>

#include "point_type.h"
#include "dashboard.h"
#include <any>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    //void signalCursorPose(QPointF pos);
    //void SendChannelMsg(const MsgId &id, const std::any &data);
    //void RecvChannelMsg(const MsgId &id, const std::any &data);
    //void updateOdomInfo(RobotState state);
    //void RestoreState();
    void SlotSetBatteryStatus(double percent, double voltage);
    //void SlotRecvImage(const std::string &location, std::shared_ptr<cv::Mat> data);

protected:
    //virtual void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    //QAction *SavePerspectiveAction = nullptr;
    //QWidgetAction *PerspectiveListAction = nullptr;
    //ChannelManager channel_manager_;
    //DashBoard *speed_dash_board_;
    //ads::CDockManager *dock_manager_;
    //ads::CDockAreaWidget *StatusDockArea;
    //ads::CDockWidget *TimelineDockWidget;
    //Display::DisplayManager *display_manager_;

    //QLabel *label_pos_map_;
    //QLabel *label_pos_scene_;
    //QThread message_thread_;
    //SpeedCtrlWidget *speed_ctrl_widget_;
    //NavGoalTableView *nav_goal_table_view_;
    QProgressBar *battery_bar_;
    QLabel *label_power_;
    //ads::CDockAreaWidget *center_docker_area_;
    //std::map<std::string, RatioLayoutedFrame *> image_frame_map_;

signals:
    //void OnRecvChannelData(const MsgId &id, const std::any &data);

private:
    void setupUi();
//    bool openChannel();
//    bool openChannel(const std::string &channel_name);
//    void closeChannel();
//    void registerChannel();
//    void SaveState();
};
#endif // MAINWINDOW_H

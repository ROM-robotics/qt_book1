#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Eigen/Dense"
#include "algorithm.h"
//#include "logger/logger.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "MainWindow init thread id " << QThread::currentThreadId() << ".\n";

    /* thread တွေကြားမှာ signal, slot ပို့ဖို့ std::string ကို meta object အဖြစ်ဖန်တီးရတယ်။ */
    qRegisterMetaType<std::string>("std::string");
//    qRegisterMetaType<RobotPose>("RobotPose");
//    qRegisterMetaType<RobotSpeed>("RobotSpeed");
//    qRegisterMetaType<RobotState>("RobotState");
//    qRegisterMetaType<OccupancyMap>("OccupancyMap");
//    qRegisterMetaType<OccupancyMap>("OccupancyMap");
//    qRegisterMetaType<LaserScan>("LaserScan");
//    qRegisterMetaType<RobotPath>("RobotPath");
//    qRegisterMetaType<MsgId>("MsgId");
//    qRegisterMetaType<std::any>("std::any");
//    qRegisterMetaType<TopologyMap>("TopologyMap");
//    qRegisterMetaType<TopologyMap::PointInfo>("TopologyMap::PointInfo");

    setupUi();
}


void MainWindow::setupUi() {
  ui->setupUi(this);
//  CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
//  CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
//  CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
//  CDockManager::setConfigFlag(CDockManager::DockAreaHasUndockButton, false);
//  CDockManager::setConfigFlag(CDockManager::DockAreaHasTabsMenuButton, false);
//  CDockManager::setConfigFlag(CDockManager::MiddleMouseButtonClosesTab, true);
//  CDockManager::setConfigFlag(CDockManager::EqualSplitOnInsertion, true);
//  CDockManager::setConfigFlag(CDockManager::ShowTabTextOnlyForActiveTab, true);
//  CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
//  dock_manager_ = new CDockManager(this);
  QVBoxLayout *center_layout = new QVBoxLayout();    // vertical
  QHBoxLayout *center_h_layout = new QHBoxLayout();  // level

  /////////////////////////////////////////////////////////////// Map toolbar
  QHBoxLayout *horizontalLayout_tools = new QHBoxLayout();
  horizontalLayout_tools->setSpacing(0);
  horizontalLayout_tools->setObjectName(
      QString::fromUtf8(" horizontalLayout_tools"));

  QToolButton *reloc_btn = new QToolButton();
  reloc_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  reloc_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:hover {"
      "   background-color: lightblue;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");

  QIcon icon4;
  icon4.addFile(QString::fromUtf8(":/images/resource/images/reloc2.svg"),
                QSize(64, 64), QIcon::Normal, QIcon::Off);
  reloc_btn->setIcon(icon4);
  reloc_btn->setText("Regulating");
  // reloc_btn->setMaximumSize(QSize(54, 54));
  reloc_btn->setIconSize(QSize(32, 32));
  horizontalLayout_tools->addWidget(reloc_btn);

  
  QIcon icon5;
  icon5.addFile(QString::fromUtf8(":/images/resource/images/edit.svg"),
                QSize(64, 64), QIcon::Normal, QIcon::Off);
  QToolButton *edit_map_btn = new QToolButton();
  edit_map_btn->setIcon(icon5);
  edit_map_btn->setText("Edit");
  // edit_map_btn->setMaximumSize(QSize(54, 54));
  edit_map_btn->setIconSize(QSize(32, 32));
  edit_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  edit_map_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:hover {"
      "   background-color: lightblue;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  horizontalLayout_tools->addWidget(edit_map_btn);

  QIcon icon6;
  icon6.addFile(QString::fromUtf8(":/images/resource/images/open.svg"),
                QSize(64, 64), QIcon::Normal, QIcon::Off);
  QToolButton *open_map_btn = new QToolButton();
  open_map_btn->setIcon(icon6);
  open_map_btn->setText("Open map");
  // open_map_btn->setMaximumSize(QSize(54, 54));
  open_map_btn->setIconSize(QSize(32, 32));
  open_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  open_map_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:hover {"
      "   background-color: lightblue;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  horizontalLayout_tools->addWidget(open_map_btn);

  QIcon icon7;
  icon7.addFile(QString::fromUtf8(":/images/resource/images/save.svg"),
                QSize(64, 64), QIcon::Normal, QIcon::Off);
  QToolButton *save_map_btn = new QToolButton();
  save_map_btn->setIcon(icon7);
  save_map_btn->setText("Save map");
  // save_map_btn->setMaximumSize(QSize(54, 54));
  save_map_btn->setIconSize(QSize(32, 32));
  save_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  save_map_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:hover {"
      "   background-color: lightblue;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  horizontalLayout_tools->addWidget(save_map_btn);
  horizontalLayout_tools->addItem(
      new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
  center_layout->addLayout(horizontalLayout_tools);

/////////////////////////////////////////////////////////////////// Battery
  battery_bar_ = new QProgressBar();
  battery_bar_->setObjectName(QString::fromUtf8("battery_bar_"));
  battery_bar_->setMaximumSize(QSize(90, 16777215));
  battery_bar_->setAutoFillBackground(true);
  battery_bar_->setStyleSheet(QString::fromUtf8(
      "QProgressBar#battery_bar_\n"
      "{\n"
      "      border:none;   /*\346\227\240\350\276\271\346\241\206*/\n"
      "      background:rgb(211, 215, 207);\n"
      "      border-radius:5px;\n"
      "      text-align:center;   "
      "/*\346\226\207\346\234\254\347\232\204\344\275\215\347\275\256*/\n"
      "      color: rgb(229, 229, 229);  "
      "/*\346\226\207\346\234\254\351\242\234\350\211\262*/\n"
      "}\n"
      " \n"
      "QProgressBar::chunk \n"
      "{\n"
      "      background-color:rgb(115, 210, 22);\n"
      "      border-radius:4px;\n"
      "}\n"
      ""));

  battery_bar_->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

  horizontalLayout_tools->addWidget(battery_bar_);

  QLabel *label_11 = new QLabel();
  label_11->setObjectName(QString::fromUtf8("label_11"));
  label_11->setMinimumSize(QSize(32, 32));
  label_11->setMaximumSize(QSize(32, 32));
  label_11->setPixmap(QPixmap(QString::fromUtf8(":/images/resource/images/power-v.png")));

  horizontalLayout_tools->addWidget(label_11);

  label_power_ = new QLabel();
  label_power_->setObjectName(QString::fromUtf8("label_power_"));
  label_power_->setMinimumSize(QSize(50, 32));
  label_power_->setMaximumSize(QSize(50, 32));
  label_power_->setStyleSheet(QString::fromUtf8(""));

  horizontalLayout_tools->addWidget(label_power_);
  SlotSetBatteryStatus(0, 0);
  

  ////////////////////////////////////////////////////////////// Edit map toolbar
  QWidget *tools_edit_map_widget = new QWidget();

  QVBoxLayout *layout_tools_edit_map = new QVBoxLayout();
  tools_edit_map_widget->setLayout(layout_tools_edit_map);
  layout_tools_edit_map->setSpacing(0);
  layout_tools_edit_map->setObjectName(
      QString::fromUtf8(" layout_tools_edit_map"));
  // Map editing Set the mouse button
  QToolButton *normal_cursor_btn = new QToolButton();
  normal_cursor_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  normal_cursor_btn->setToolTip("mouse");
  normal_cursor_btn->setCursor(Qt::PointingHandCursor);
  normal_cursor_btn->setIconSize(QSize(32, 32));

  QIcon pose_tool_btn_icon;
  pose_tool_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/cursor_point_btn.svg"),
                             QSize(), QIcon::Normal, QIcon::Off);
  normal_cursor_btn->setIcon(pose_tool_btn_icon);
  layout_tools_edit_map->addWidget(normal_cursor_btn);

  // Add point button
  QToolButton *add_point_btn = new QToolButton();
  add_point_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  add_point_btn->setToolTip("Add work site");
  add_point_btn->setCursor(Qt::PointingHandCursor);
  add_point_btn->setIconSize(QSize(32, 32));

  QIcon add_point_btn_icon;
  add_point_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/point_btn.svg"),
                             QSize(), QIcon::Normal, QIcon::Off);
  add_point_btn->setIcon(add_point_btn_icon);
  layout_tools_edit_map->addWidget(add_point_btn);

  QToolButton *add_topology_path_btn = new QToolButton();
  add_topology_path_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  add_topology_path_btn->setToolTip("Connection station point");
  add_topology_path_btn->setCursor(Qt::PointingHandCursor);
  add_topology_path_btn->setIconSize(QSize(32, 32));

  QIcon add_topology_path_btn_icon;
  add_topology_path_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/topo_link_btn.svg"),
                                     QSize(), QIcon::Normal, QIcon::Off);
  add_topology_path_btn->setIcon(add_topology_path_btn_icon);
  layout_tools_edit_map->addWidget(add_topology_path_btn);
  //TODO Topology point connection
  add_topology_path_btn->setEnabled(false);
  // Add area button
  QToolButton *add_region_btn = new QToolButton();
  add_region_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  add_region_btn->setToolTip("Add area");
  add_region_btn->setCursor(Qt::PointingHandCursor);
  add_region_btn->setIconSize(QSize(32, 32));

  QIcon add_region_btn_icon;
  add_region_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/region_btn.svg"),
                              QSize(), QIcon::Normal, QIcon::Off);
  add_region_btn->setIcon(add_region_btn_icon);
  add_region_btn->setEnabled(false);
  layout_tools_edit_map->addWidget(add_region_btn);

  // Separate
  QFrame *separator = new QFrame();
  separator->setFrameShape(QFrame::HLine);
  separator->setFrameShadow(QFrame::Sunken);

  // Add the segmentation symbol to the layout
  layout_tools_edit_map->addWidget(separator);

  // Rubber rubbing button
  QToolButton *erase_btn = new QToolButton();
  erase_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  erase_btn->setToolTip("Eraser");
  erase_btn->setCursor(Qt::PointingHandCursor);
  erase_btn->setIconSize(QSize(32, 32));

  QIcon erase_btn_icon;
  erase_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/erase_btn.svg"),
                         QSize(), QIcon::Normal, QIcon::Off);
  erase_btn->setIcon(erase_btn_icon);
  layout_tools_edit_map->addWidget(erase_btn);
  // Brush button
  QToolButton *draw_pen_btn = new QToolButton();
  draw_pen_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  draw_pen_btn->setToolTip("line");
  draw_pen_btn->setCursor(Qt::PointingHandCursor);
  draw_pen_btn->setIconSize(QSize(32, 32));

  QIcon draw_pen_btn_icon;
  draw_pen_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/pen.svg"),
                            QSize(), QIcon::Normal, QIcon::Off);
  draw_pen_btn->setIcon(draw_pen_btn_icon);

  layout_tools_edit_map->addWidget(draw_pen_btn);

  // Line button
  QToolButton *draw_line_btn = new QToolButton();
  draw_line_btn->setStyleSheet(
      "QToolButton {"
      "   border: none;"
      "   background-color: transparent;"
      "}"
      "QToolButton:pressed {"
      "   background-color: lightblue;"
      "}");
  draw_line_btn->setToolTip("line");
  draw_line_btn->setCursor(Qt::PointingHandCursor);
  draw_line_btn->setIconSize(QSize(32, 32));

  QIcon draw_line_btn_icon;
  draw_line_btn_icon.addFile(QString::fromUtf8(":/images/resource/images/line_btn.svg"),
                             QSize(), QIcon::Normal, QIcon::Off);
  draw_line_btn->setIcon(draw_line_btn_icon);

  layout_tools_edit_map->addWidget(draw_line_btn);

  layout_tools_edit_map->addItem(
      new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
  tools_edit_map_widget->hide();
  center_h_layout->addWidget(tools_edit_map_widget);
  center_layout->addLayout(center_h_layout);


  ///////////////////////////////////////////////////////////////////////// Map display

  display_manager_ = new Display::DisplayManager();
  center_h_layout->addWidget(display_manager_->GetViewPtr());

  ////////////////////////////////////////////////////////////////////////// Coordinate display
  QHBoxLayout *horizontalLayout_12 = new QHBoxLayout();
  horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
  QLabel *label = new QLabel();
  label->setText("map:");
  label->setObjectName(QString::fromUtf8("label"));

  horizontalLayout_12->addWidget(label);

  label_pos_map_ = new QLabel();
  label_pos_map_->setObjectName(QString::fromUtf8("label_pos_map_"));
  label_pos_map_->setStyleSheet(QString::fromUtf8(""));

  horizontalLayout_12->addWidget(label_pos_map_);

  QLabel *label_5 = new QLabel();
  label_5->setText("scene:");

  label_5->setObjectName(QString::fromUtf8("label_5"));

  horizontalLayout_12->addWidget(label_5);

  label_pos_scene_ = new QLabel();
  label_pos_scene_->setObjectName(QString::fromUtf8("label_pos_scene_"));

  horizontalLayout_12->addWidget(label_pos_scene_);

  horizontalLayout_12->addItem(
      new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

  center_layout->addLayout(horizontalLayout_12);

} // end CONSTRUTOR

void MainWindow::SlotSetBatteryStatus(double percent, double voltage) {
  battery_bar_->setValue(percent);
  label_power_->setText(QString::number(voltage, 'f', 2) + "V");
}

MainWindow::~MainWindow()
{
    delete ui;
}


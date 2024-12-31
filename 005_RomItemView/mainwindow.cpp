#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->addItem("ROM Dynamics Item");

    /// Another way for listWidget
    QListWidgetItem* listItem = new QListWidgetItem;
    listItem->setText("DeepBlue AI Lab Item");
    listItem->setData(100, 1000);
    ui->listWidget->addItem(listItem);

    /// Tree
    QTreeWidgetItem* treeItem = new QTreeWidgetItem;
    treeItem->setText(0, "My Test Item");
    ui->treeWidget->addTopLevelItem(treeItem);

        QTreeWidgetItem* treeItem2 = new QTreeWidgetItem;
        treeItem2->setText(0, "My Test Subitem");
        treeItem->addChild(treeItem2);

    /// Table
    QTableWidgetItem* tableItem = new QTableWidgetItem;
    tableItem->setText("Myanmar");
    ui->tableWidget->setItem(0, 0, tableItem);

    QTableWidgetItem* tableItem2 = new QTableWidgetItem;
    tableItem2->setText("English");
    ui->tableWidget->setItem(1, 2, tableItem2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


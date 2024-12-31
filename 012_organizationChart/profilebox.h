#ifndef PROFILEBOX_H
#define PROFILEBOX_H

#include "mainwindow.h"

class MainWindow;

class profileBox : public QGraphicsItemGroup
{
public:
   explicit profileBox(QGraphicsItem* parent = nullptr);
   void init(QString name, MainWindow *window, QGraphicsScene* scene);

private:
   MainWindow* mainWindow;
   QGraphicsTextItem* nameTag;
};

#endif // PROFILEBOX_H

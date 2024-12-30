#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class clientDialog;
}

class clientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit clientDialog(QWidget *parent = nullptr);
    ~clientDialog();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::clientDialog *ui;
};

#endif // CLIENTDIALOG_H

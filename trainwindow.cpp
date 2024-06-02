#include "trainwindow.h"
#include "ui_trainwindow.h"
#include <QDebug>
#include <QApplication>

TrainWindow::TrainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_4->setReadOnly(true);
}

TrainWindow::~TrainWindow()
{
    delete ui;
}

void TrainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug() << arg1;
    if (arg1 == 2) {
        ui->lineEdit_4->setReadOnly(false);
    } else {
        ui->lineEdit_4->setReadOnly(true);
    }
}

void TrainWindow::on_pushButton_clicked()
{
    QApplication::quit();
}

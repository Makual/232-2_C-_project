#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include "mainwindow.h"
#include "mlp.h"

#include <QApplication>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <inferencewindow.h>
#include <trainwindow.h>


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
//    std::cout << "gggggggggggggggg\n";
//    ui->label->setText("Кнопка нажата");
    QApplication::quit();
}

void MainWindow::on_pushButton_2_clicked()
{
//    QMessageBox::about(this, "Заголовок", "Просто текст внутри");
//    QMessageBox::critical(this, "Заголовок", "Просто текст внутри");
//    QMessageBox::information(this, "Заголовок", "Просто текст внутри");
//    QMessageBox::warning(this, "Заголовок", "Просто текст внутри");

//    QMessageBox::StandardButton reply = QMessageBox::question(this, "Заголовок", "Вы хотите загрузить файл?", QMessageBox::Yes | QMessageBox::No);
//    if (reply == QMessageBox::Yes) {

//    } else {
//        qDebug() << "Кнопка нет была нажата";
//    }

    hide();
    InferenceWindow window;
    window.setModal(true);
    window.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    hide();
    TrainWindow window;
    window.setModal(true);
    window.exec();
}

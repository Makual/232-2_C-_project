#include "trainwindow.h"
#include "ui_trainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include "mlp.h"
#include "inferencewindow.h"
#include "ui_inferencewindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <mainwindow.h>

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

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"



std::string path1;
std::string path2;

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

void TrainWindow::on_pushButton_2_clicked()
{
    QString Q_imageSize = ui->lineEdit->text();
    QString Q_valSize = ui->lineEdit_2->text();
    QString Q_epochsQty = ui->lineEdit_3->text();
    QString Q_learningRate = ui->lineEdit_4->text();

    std::string imageSize_str = Q_imageSize.toStdString();
    std::string valSize_str = Q_valSize.toStdString();
    std::string epochsQty_str = Q_epochsQty.toStdString();
    std::string learningRate_str = Q_learningRate.toStdString();

    int imageSize = stoi (imageSize_str);
    double valSize = stod (valSize_str);
    int epochsQty = stoi (epochsQty_str);
    double learningRate;

    if (ui->checkBox->isChecked()) {
        learningRate = stod (learningRate_str); //0.01
    } else {
        learningRate = 0.01;
    }


    std::vector<std::vector<double>> images;
    std::vector<std::vector<double>> labels;
    loadImagesAndLabels(path2, path1, images, labels, imageSize);

    int valQty = images.size() * valSize;

    std::vector<std::vector<double>> trainImages(images.begin() + valSize, images.end());
    std::vector<std::vector<double>> trainLabels(labels.begin() + valSize, labels.end());
    std::vector<std::vector<double>> valImages(images.begin(), images.begin() + valQty);
    std::vector<std::vector<double>> valLabels(labels.begin(), labels.begin() + valQty);

    std::vector<int> layerSizes = { imageSize / 2 * imageSize / 2 * 60, 128, 64, 32, 10 };
    std::vector<int> kernelSizes = { 7, 5, 3, 1};
    std::vector<int> numKernels = { 15, 15 , 15, 15};
    MLP mlp(layerSizes, kernelSizes, numKernels, imageSize, imageSize);
    mlp.train(trainImages, trainLabels, valImages, valLabels, learningRate, epochsQty);
    mlp.saveWeights("mlp_weights_mnist_60_kernels.txt");
}

void TrainWindow::on_pushButton_3_clicked()
{
    // images path
    QString str = QFileDialog::getExistingDirectory(0, "Select", "");
    qDebug() << str;
    path1 = str.toStdString();
}

void TrainWindow::on_pushButton_4_clicked()
{
    // labels path
    QString str = QFileDialog::getOpenFileName(0, "Select", "", "*.csv");
    qDebug() << str;
    path2 = str.toStdString();
}

#include "inferencewindow.h"
#include "ui_inferencewindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <mainwindow.h>
#include <QVBoxLayout>
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

std::vector<double> preprocessImage(const std::string& imagePath, int targetWidth, int targetHeight) {
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 1); 
    if (!data) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        exit(1);
    }

    std::vector<unsigned char> resizedData(targetWidth * targetHeight);
    stbir_resize_uint8(data, width, height, 0, resizedData.data(), targetWidth, targetHeight, 0, 1);
    stbi_image_free(data);

    std::vector<double> image(resizedData.begin(), resizedData.end());
    for (double& pixel : image) {
        pixel /= 255.0; 
    }

    return image;
}

InferenceWindow::InferenceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InferenceWindow)
{
    ui->setupUi(this);
    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(layout);
}

InferenceWindow::~InferenceWindow()
{
    delete ui;
}

void InferenceWindow::on_pushButton_2_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Открыть", "", "*.jpg *.png");
    qDebug() << str;
    std::string imagePath = str.toStdString();

    const int imageSize = 32;


    std::vector<double> image = preprocessImage(imagePath, imageSize, imageSize);


    std::vector<int> layerSizes = { imageSize / 2 * imageSize / 2 * 60, 128, 64, 32, 10 };
    std::vector<int> kernelSizes = { 7, 5, 3, 1 };
    std::vector<int> numKernels = { 15, 15, 15, 15 };
    MLP mlp(layerSizes, kernelSizes, numKernels, imageSize, imageSize);
    mlp.loadWeights("C:/Users/Makual/Downloads/232-2_CPP_project/232-2_CPP_project/232-2_CPP_project/mlp_weights_mnist_60_kernels_30.txt");


    std::vector<double> output = mlp.forward(image);


    clearScrollArea();


    appendTextToScrollArea("Class probabilities:");
    for (size_t i = 0; i < output.size(); ++i) {
        appendTextToScrollArea(QString("Class %1: %2").arg(i).arg(output[i]));
    }
}

void InferenceWindow::on_pushButton_clicked()
{
    QApplication::quit();
}

void InferenceWindow::appendTextToScrollArea(const QString &text)
{
    QLabel *label = new QLabel(text, this);
    label->setWordWrap(true);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (layout) {
        layout->addWidget(label);
    }
}

void InferenceWindow::clearScrollArea()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (layout) {
        while (QLayoutItem* item = layout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
    }
}

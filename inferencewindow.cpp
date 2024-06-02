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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

std::vector<double> preprocessImage(const std::string& imagePath, int targetWidth, int targetHeight) {
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 1); // Load image as grayscale
    if (!data) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        exit(1);
    }

    std::vector<unsigned char> resizedData(targetWidth * targetHeight);
    stbir_resize_uint8(data, width, height, 0, resizedData.data(), targetWidth, targetHeight, 0, 1);
    stbi_image_free(data);

    std::vector<double> image(resizedData.begin(), resizedData.end());
    for (double& pixel : image) {
        pixel /= 255.0; // Normalize pixel values to [0, 1]
    }

    return image;
}

InferenceWindow::InferenceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InferenceWindow)
{
    ui->setupUi(this);
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

//    std::string imagePath = "F:/!Tima/!AI/qt/CPP_project/test.jpg";
//    std::string imagePath = "F:/!Tima/!AI/qt/CPP_project/number_two.png";

    const int imageSize = 32;

    // Preprocess the image
    std::vector<double> image = preprocessImage(imagePath, imageSize, imageSize);

    // Initialize the MLP
    std::vector<int> layerSizes = { imageSize / 2 * imageSize / 2 * 60, 128, 64, 32, 10 };
    std::vector<int> kernelSizes = { 7, 5, 3, 1 };
    std::vector<int> numKernels = { 15, 15, 15, 15 };
    MLP mlp(layerSizes, kernelSizes, numKernels, imageSize, imageSize);
    mlp.loadWeights("F:/!Tima/!AI/qt/CPP_project/mlp_weights_mnist_60_kernels_30.txt");

    // Perform forward pass
    std::vector<double> output = mlp.forward(image);

    // Print the probabilities for each class
    std::cout << "Class probabilities: " << std::endl;
    for (size_t i = 0; i < output.size(); ++i) {
        std::cout << "Class " << i << ": " << output[i] << std::endl;
    }

}

void InferenceWindow::on_pushButton_clicked()
{
//    MainWindow window;
//    window.setModal(true);
//    window.exec();
    QApplication::quit();
}

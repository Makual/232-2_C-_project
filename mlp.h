#ifndef MLP_H
#define MLP_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>

#include "conv_layer.h"

double sigmoid(double x);
double sigmoid_derivative(double x);
std::vector<double> softmax(const std::vector<double>& x);

class MLP {
public:
    MLP(const std::vector<int>& layerSizes, const std::vector<int>& kernelSizes, const std::vector<int>& numKernels, int imageWidth, int imageHeight);
    std::vector<double> forward(const std::vector<double>& input);
    void backward(const std::vector<double>& input, const std::vector<double>& target, double learningRate);
    void train(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& targets,
               const std::vector<std::vector<double>>& valInputs, const std::vector<std::vector<double>>& valTargets,
               double learningRate, int epochs);
    double validate(const std::vector<std::vector<double>>& valInputs, const std::vector<std::vector<double>>& valTargets);
    void saveWeights(const std::string& filename);
    void loadWeights(const std::string& filename);

private:
    std::vector<int> layerSizes;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;
    std::vector<std::vector<double>> activations;
    ConvLayer convLayer;

    void initializeWeights();
    double computeLoss(const std::vector<double>& output, const std::vector<double>& target);
};

void loadImagesAndLabels(const std::string& csvFile, const std::string& imageDir, std::vector<std::vector<double>>& images, std::vector<std::vector<double>>& labels, int imageSize);

#endif // MLP_H

#include "conv_layer.h"

Kernel::Kernel(int size) {
    weights.resize(size, std::vector<double>(size));
    initializeWeights();
}

void Kernel::initializeWeights() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    for (auto& row : weights) {
        for (auto& weight : row) {
            weight = distribution(generator);
        }
    }
}

void convolve(unsigned char* image, int width, int height, const Kernel& kernel, double* output) {
    int kernelSize = kernel.weights.size();
    int padSize = kernelSize / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = 0.0;
            for (int ky = -padSize; ky <= padSize; ++ky) {
                for (int kx = -padSize; kx <= padSize; ++kx) {
                    int iy = std::min(std::max(y + ky, 0), height - 1);
                    int ix = std::min(std::max(x + kx, 0), width - 1);
                    sum += image[ix + iy * width] * kernel.weights[ky + padSize][kx + padSize];
                }
            }
            output[x + y * width] = sum;
        }
    }
}

void maxPooling(double* input, int width, int height, int poolSize, double* output) {
    int newWidth = width / poolSize;
    int newHeight = height / poolSize;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double maxVal = -std::numeric_limits<double>::infinity();
            for (int dy = 0; dy < poolSize; ++dy) {
                for (int dx = 0; dx < poolSize; ++dx) {
                    int iy = y * poolSize + dy;
                    int ix = x * poolSize + dx;
                    if (iy < height && ix < width) {
                        maxVal = std::max(maxVal, input[ix + iy * width]);
                    }
                }
            }
            output[x + y * newWidth] = maxVal;
        }
    }
}

ConvLayer::ConvLayer(const std::vector<int>& kernelSizes, const std::vector<int>& numKernels, int inputWidth, int inputHeight)
    : kernelSizes(kernelSizes), numKernels(numKernels), inputWidth(inputWidth), inputHeight(inputHeight) {
    initializeKernels();
}

std::vector<double> ConvLayer::forward(const std::vector<double>& input) {
    int outputSize = inputWidth * inputHeight;
    std::vector<double> convOutput(totalKernels * outputSize);
    std::vector<unsigned char> inputImage(input.begin(), input.end());

    int offset = 0;
    for (size_t k = 0; k < kernels.size(); ++k) {
        for (int i = 0; i < numKernels[k]; ++i) {
            convolve(inputImage.data(), inputWidth, inputHeight, kernels[k][i], convOutput.data() + (offset + i) * outputSize);
        }
        offset += numKernels[k];
    }

    std::vector<double> pooledOutput(totalKernels * (inputWidth / 2) * (inputHeight / 2));
    offset = 0;
    for (size_t k = 0; k < kernels.size(); ++k) {
        for (int i = 0; i < numKernels[k]; ++i) {
            maxPooling(convOutput.data() + (offset + i) * outputSize, inputWidth, inputHeight, 2, pooledOutput.data() + (offset + i) * (inputWidth / 2) * (inputHeight / 2));
        }
        offset += numKernels[k];
    }

    return pooledOutput;
}

void ConvLayer::initializeKernels() {
    totalKernels = 0;
    for (size_t i = 0; i < kernelSizes.size(); ++i) {
        std::vector<Kernel> kernelGroup;
        for (int j = 0; j < numKernels[i]; ++j) {
            kernelGroup.emplace_back(kernelSizes[i]);
        }
        kernels.push_back(kernelGroup);
        totalKernels += numKernels[i];
    }
}

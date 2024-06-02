#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include <vector>
#include <algorithm>
#include <random>

struct Kernel {
    std::vector<std::vector<double>> weights;

    Kernel(int size);
    void initializeWeights();
};

void convolve(unsigned char* image, int width, int height, const Kernel& kernel, double* output);
void maxPooling(double* input, int width, int height, int poolSize, double* output);

class ConvLayer {
public:
    ConvLayer(const std::vector<int>& kernelSizes, const std::vector<int>& numKernels, int inputWidth, int inputHeight);
    std::vector<double> forward(const std::vector<double>& input);

private:
    std::vector<int> kernelSizes;
    std::vector<int> numKernels;
    int inputWidth;
    int inputHeight;
    std::vector<std::vector<Kernel>> kernels;
    int totalKernels;

    void initializeKernels();
};

#endif // CONV_LAYER_H

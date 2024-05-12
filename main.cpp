#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <algorithm>

void convolve(unsigned char* image, int width, int height, const std::vector<std::vector<double>>& kernel, unsigned char* output) {
    int kernelWidth = kernel[0].size();
    int kernelHeight = kernel.size();
    int padWidth = kernelWidth / 2;
    int padHeight = kernelHeight / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = 0.0;
            for (int ky = -padHeight; ky <= padHeight; ++ky) {
                for (int kx = -padWidth; kx <= padWidth; ++kx) {
                    int iy = std::min(std::max(y + ky, 0), height - 1);
                    int ix = std::min(std::max(x + kx, 0), width - 1);
                    sum += image[ix + iy * width] * kernel[ky + padHeight][kx + padWidth];
                }
            }
            output[x + y * width] = std::min(std::max(int(sum), 0), 255);
        }
    }
}

void maxPooling(unsigned char* input, int width, int height, int poolSize, unsigned char* output) {
    int newWidth = width / poolSize;
    int newHeight = height / poolSize;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            unsigned char maxVal = 0;
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

int main() {
    int width, height, channels;
    const char* inputImagePath = "input.jpg";
    const char* outputImagePath = "output.jpg";

    unsigned char* input_image = stbi_load(inputImagePath, &width, &height, &channels, 1);
    if (input_image == nullptr) {
        std::cerr << "Error loading the image" << std::endl;
        return 1;
    }

    unsigned char* intermediate_image = new unsigned char[width * height];
    unsigned char* output_image = new unsigned char[width * height / 4]; // Размер уменьшится после пулинга

    std::vector<std::vector<double>> kernel = {
        {1, 0, -1},
        {1, 0, -1},
        {1, 0, -1}
    };

    //std::vector<std::vector<double>> kernel = {
    //    {-1, -1, -1},
    //    {-1, 8, -1},
    //    {-1, -1, -1}
    //};

    convolve(input_image, width, height, kernel, intermediate_image);
    maxPooling(intermediate_image, width, height, 2, output_image);

    int newWidth = width / 2;
    int newHeight = height / 2;
    unsigned char* final_output = new unsigned char[newWidth * newHeight / 4];
    convolve(output_image, newWidth, newHeight, kernel, intermediate_image);
    maxPooling(intermediate_image, newWidth, newHeight, 2, final_output);

    stbi_write_jpg(outputImagePath, newWidth / 2, newHeight / 2, 1, final_output, 100);
    std::cout << "Output image saved." << std::endl;

    stbi_image_free(input_image);
    delete[] intermediate_image;
    delete[] output_image;
    delete[] final_output;

    return 0;
}

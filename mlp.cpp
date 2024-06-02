#include "mlp.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1 - x);
}

std::vector<double> softmax(const std::vector<double>& x) {
    std::vector<double> result(x.size());
    double maxVal = *std::max_element(x.begin(), x.end());
    double sum = 0.0;

    for (double val : x) {
        sum += std::exp(val - maxVal);
    }

    for (size_t i = 0; i < x.size(); ++i) {
        result[i] = std::exp(x[i] - maxVal) / sum;
    }

    return result;
}

MLP::MLP(const std::vector<int>& layerSizes, const std::vector<int>& kernelSizes, const std::vector<int>& numKernels, int imageWidth, int imageHeight)
    : layerSizes(layerSizes), convLayer(kernelSizes, numKernels, imageWidth, imageHeight) {
    initializeWeights();
}

std::vector<double> MLP::forward(const std::vector<double>& input) {
    std::vector<double> convOutput = convLayer.forward(input);
    activations[0] = convOutput;
    for (size_t i = 1; i < layerSizes.size() - 1; ++i) {
        for (int j = 0; j < layerSizes[i]; ++j) {
            double sum = 0.0;
            for (int k = 0; k < layerSizes[i-1]; ++k) {
                sum += activations[i-1][k] * weights[i-1][j][k];
            }
            sum += biases[i-1][j];
            activations[i][j] = sigmoid(sum);
        }
    }

    // Softmax activation for the output layer
    for (int j = 0; j < layerSizes.back(); ++j) {
        double sum = 0.0;
        for (int k = 0; k < layerSizes[layerSizes.size()-2]; ++k) {
            sum += activations[layerSizes.size()-2][k] * weights[layerSizes.size()-2][j][k];
        }
        sum += biases[layerSizes.size()-2][j];
        activations.back()[j] = sum;
    }
    activations.back() = softmax(activations.back());

    return activations.back();
}

void MLP::backward(const std::vector<double>& input, const std::vector<double>& target, double learningRate) {
    std::vector<double> output = forward(input);
    std::vector<std::vector<double>> deltas(layerSizes.size());
    for (size_t i = 0; i < deltas.size(); ++i) {
        deltas[i].resize(layerSizes[i]);
    }

    // Calculate deltas for the output layer (using cross-entropy loss derivative)
    for (int i = layerSizes.back() - 1; i >= 0; --i) {
        deltas.back()[i] = output[i] - target[i];
    }

    // Calculate deltas for the hidden layers
    for (int i = layerSizes.size() - 2; i >= 0; --i) {
        for (int j = 0; j < layerSizes[i]; ++j) {
            double error = 0.0;
            for (int k = 0; k < layerSizes[i+1]; ++k) {
                error += deltas[i+1][k] * weights[i][k][j];
            }
            deltas[i][j] = error * sigmoid_derivative(activations[i][j]);
        }
    }

    // Update weights and biases
    for (int i = 0; i < weights.size(); ++i) {
        for (int j = 0; j < weights[i].size(); ++j) {
            for (int k = 0; k < weights[i][j].size(); ++k) {
                weights[i][j][k] -= learningRate * deltas[i+1][j] * activations[i][k];
            }
            biases[i][j] -= learningRate * deltas[i+1][j];
        }
    }
}

void MLP::train(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& targets,
                const std::vector<std::vector<double>>& valInputs, const std::vector<std::vector<double>>& valTargets,
                double learningRate, int epochs) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        double totalLoss = 0.0;
        for (size_t i = 0; i < inputs.size(); ++i) {
            backward(inputs[i], targets[i], learningRate);
            totalLoss += computeLoss(forward(inputs[i]), targets[i]);
        }
        std::cout << "Epoch " << epoch + 1 << "/" << epochs << ", Loss: " << totalLoss / inputs.size();

        double accuracy = validate(valInputs, valTargets);
        std::cout << ", Validation Accuracy: " << accuracy << std::endl;
    }
}

double MLP::validate(const std::vector<std::vector<double>>& valInputs, const std::vector<std::vector<double>>& valTargets) {
    int correct = 0;
    for (size_t i = 0; i < valInputs.size(); ++i) {
        std::vector<double> output = forward(valInputs[i]);
        int predicted = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
        int actual = std::distance(valTargets[i].begin(), std::max_element(valTargets[i].begin(), valTargets[i].end()));
        if (predicted == actual) {
            ++correct;
        }
    }
    return static_cast<double>(correct) / valInputs.size();
}

void MLP::saveWeights(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing weights." << std::endl;
        return;
    }

    for (const auto& layerWeights : weights) {
        for (const auto& neuronWeights : layerWeights) {
            for (double weight : neuronWeights) {
                file << weight << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    for (const auto& layerBiases : biases) {
        for (double bias : layerBiases) {
            file << bias << " ";
        }
        file << "\n";
    }

    file.close();
}

void MLP::loadWeights(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading weights." << std::endl;
        return;
    }

    for (auto& layerWeights : weights) {
        for (auto& neuronWeights : layerWeights) {
            for (double& weight : neuronWeights) {
                file >> weight;
            }
        }
    }

    for (auto& layerBiases : biases) {
        for (double& bias : layerBiases) {
            file >> bias;
        }
    }

    file.close();
}

void MLP::initializeWeights() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    weights.resize(layerSizes.size() - 1);
    biases.resize(layerSizes.size() - 1);
    activations.resize(layerSizes.size());

    for (size_t i = 0; i < weights.size(); ++i) {
        weights[i].resize(layerSizes[i+1], std::vector<double>(layerSizes[i]));
        biases[i].resize(layerSizes[i+1]);
        activations[i+1].resize(layerSizes[i+1]);

        for (int j = 0; j < layerSizes[i+1]; ++j) {
            for (int k = 0; k < layerSizes[i]; ++k) {
                weights[i][j][k] = distribution(generator);
            }
            biases[i][j] = distribution(generator);
        }
    }
}

double MLP::computeLoss(const std::vector<double>& output, const std::vector<double>& target) {
    double loss = 0.0;
    for (size_t i = 0; i < output.size(); ++i) {
        loss -= target[i] * std::log(output[i] + 1e-15); // Adding a small value to avoid log(0)
    }
    return loss;
}

void loadImagesAndLabels(const std::string& csvFile, const std::string& imageDir, std::vector<std::vector<double>>& images, std::vector<std::vector<double>>& labels, int imageSize) {
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Error opening CSV file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string imagePath, label, id;
        if (!std::getline(ss, id, ',')) continue;
        if (!std::getline(ss, label, ',')) continue;
        if (!std::getline(ss, imagePath, ',')) continue;

        std::string fullImagePath = "C:/Users/Makual/Documents/test/" + imageDir + "/" + imagePath;

        int width, height, channels;
        unsigned char* input_image = stbi_load(fullImagePath.c_str(), &width, &height, &channels, 1);
        if (input_image == nullptr) {
            std::cerr << "Error loading image: " << fullImagePath << std::endl;
            continue;
        }

        unsigned char* resized_image = new unsigned char[imageSize * imageSize];
        stbir_resize_uint8(input_image, width, height, 0, resized_image, imageSize, imageSize, 0, 1);
        stbi_image_free(input_image);

        std::vector<double> imageVector(imageSize * imageSize);
        for (int i = 0; i < imageSize * imageSize; ++i) {
            imageVector[i] = resized_image[i] / 255.0;
        }
        delete[] resized_image;

        images.push_back(imageVector);

        std::vector<double> labelVector(10, 0.0); // Assuming 10 classes
        try {
            int labelIndex = std::stoi(label);
            if (labelIndex >= 0 && labelIndex < 10) {
                labelVector[labelIndex] = 1.0;
            } else {
                std::cerr << "Invalid label value: " << label << std::endl;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error converting label to integer: " << label << ", what(): " << e.what() << std::endl;
            continue;
        } catch (const std::out_of_range& e) {
            std::cerr << "Label value out of range: " << label << ", what(): " << e.what() << std::endl;
            continue;
        }

        labels.push_back(labelVector);
    }
}

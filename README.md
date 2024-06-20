# 232-2_CPP_project
Authors: Kuznetsov Maxim, Silaev Timofey 232-2

Technical specification:
The goal of the project is to create a desktop application for recognizing handwritten numbers

Description:
The base of our application will be a hand-written library, the basis of which will be a multi-layered perceptron. A script for open data training and a GUI interface with the ability to download and further recognize digit images will also be implemented.
The interface will be implemented through the QT library. It is planned to use the MNIST dataset to train the model. The training will take place in advance, users will work in the application already with a trained model, but it will be possible to run the training script and retrain the model.

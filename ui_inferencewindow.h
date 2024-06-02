/********************************************************************************
** Form generated from reading UI file 'inferencewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFERENCEWINDOW_H
#define UI_INFERENCEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_InferenceWindow
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *InferenceWindow)
    {
        if (InferenceWindow->objectName().isEmpty())
            InferenceWindow->setObjectName(QString::fromUtf8("InferenceWindow"));
        InferenceWindow->resize(400, 300);
        pushButton = new QPushButton(InferenceWindow);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(320, 270, 75, 23));
        pushButton_2 = new QPushButton(InferenceWindow);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 110, 151, 23));

        retranslateUi(InferenceWindow);

        QMetaObject::connectSlotsByName(InferenceWindow);
    } // setupUi

    void retranslateUi(QDialog *InferenceWindow)
    {
        InferenceWindow->setWindowTitle(QCoreApplication::translate("InferenceWindow", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("InferenceWindow", "Exit", nullptr));
        pushButton_2->setText(QCoreApplication::translate("InferenceWindow", "Select a file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InferenceWindow: public Ui_InferenceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFERENCEWINDOW_H

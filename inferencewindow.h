#ifndef INFERENCEWINDOW_H
#define INFERENCEWINDOW_H

#include <QDialog>

namespace Ui {
class InferenceWindow;
}

class InferenceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InferenceWindow(QWidget *parent = nullptr);
    ~InferenceWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::InferenceWindow *ui;
};

#endif // INFERENCEWINDOW_H

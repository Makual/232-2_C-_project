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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

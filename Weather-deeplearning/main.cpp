// main.cpp

#include "mainwindow.h"
#include "neuron.h"
#include "matrix.h"
#include "neuralnetwork.h"
#include "layer.h"
#include "utils/Math.h"
#include "utils/Misc.h"
#include "json.h"

#include <QApplication>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <ostream>
#include <time.h>
#include <Python.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}

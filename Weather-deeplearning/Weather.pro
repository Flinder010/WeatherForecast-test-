QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Math.cpp \
    Misc.cpp \
    layer.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    matrixtovector.cpp \
    multiplymatrix.cpp \
    neuralnetwork.cpp \
    neuron.cpp \
    weatherai.cpp \
    weatheraipython.cpp

HEADERS += \
    json.h \
    layer.h \
    mainwindow.h \
    matrix.h \
    matrixtovector.h \
    multiplymatrix.h \
    neuralnetwork.h \
    neuron.h \
    utils/Math.h \
    utils/Misc.h \
    weatherai.h \
    weatheraipython.h \
    weatherdata.h \
    weathertool.h

FORMS += \
    mainwindow.ui \
    weatherai.ui \
    weatheraipython.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Figure.qrc \
    citycode.qrc

INCLUDEPATH += -I D:\Anaconda\envs\DL\include
LIBS += -LD:\Anaconda\envs\DL\libs -lpython39

DISTFILES += \
    Data/label_data.csv \
    Data/sample_data.csv \
    Data/standard_label_data.csv \
    Data/standard_sample_data.csv \
    config.json \
    saveWeights/saveWeights.json

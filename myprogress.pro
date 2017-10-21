#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T15:51:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = myprogress
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    buffers.cpp \
    cypress_device.cpp \
    data_frame.cpp \
    info_frame.cpp \
    usb_helper.cpp \
    usb_manager.cpp \
    global.cpp \
    smoothing.cpp \
    DerivativePeaksValleysFinder.cpp \
    Engine.cpp \
    Engine2.cpp \
    engine3.cpp \
    PeakSearchFirstDerivative.cpp \
    PeaksValleysFinder.cpp \
    SigmaCalc.cpp \
    chn.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    cypress_device.h \
    data_frame.h \
    info_frame.h \
    usb_manager.h \
    global.h \
    Engine.h \
    buffers.h \
    chn.h

FORMS    += mainwindow.ui \

LIBS += -lQt5Concurrent

LIBS +=  -L"C:\\now\\libusb-1.0.20\\libusb-1.0.20\\MinGW32\\dll\\" -llibusb-1.0
INCLUDEPATH+= C:\now\libusb-1.0.20\libusb-1.0.20\include\

#LIBS += /usr/lib/x86_64-linux-gnu/libusb-1.0.so
#INCLUDEPATH+= /usr/include/libusb-1.0/



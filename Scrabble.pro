TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    console/consolecolor.cpp \
    console/letter.cpp \
    console/widget.cpp

HEADERS += \
    console/common.h \
    console/consolecolor.h \
    console/letter.h \
    console/widget.h

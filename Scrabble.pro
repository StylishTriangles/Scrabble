TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    console/consolecolor.cpp \
    console/letter.cpp \
    console/widget.cpp \
    game/game.cpp \
    game/letterbag.cpp \
    game/tile.cpp \
    console/cursor.cpp \
    events/eventmanager.cpp \
    events/keyevent.cpp \
    events/event.cpp

HEADERS += \
    console/consolecolor.h \
    console/letter.h \
    console/widget.h \
    game/game.h \
    game/letterbag.h \
    game/tile.h \
    common.h \
    console/cursor.h \
    events/eventmanager.h \
    events/keyevent.h \
    events/event.h

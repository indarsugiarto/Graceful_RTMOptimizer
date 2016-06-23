TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    SpiNQ.c \
    main.c \
    eventhandler.c \
    init.c

INCLUDEPATH += /opt/spinnaker_tools_134/include

DISTFILES += \
    Makefile \
    README

HEADERS += \
    SpiNQ.h \
    stdspinapi.h

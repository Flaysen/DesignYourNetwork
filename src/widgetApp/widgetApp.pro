QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../common.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

include(../core/core.pri)

RESOURCES += \
    resources.qrc

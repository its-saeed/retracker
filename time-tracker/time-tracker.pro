#-------------------------------------------------
#
# Project created by QtCreator 2018-11-07T12:58:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = time-tracker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    IssueTableWidget.cpp \
    UserPassDialog.cpp \
    TimePickerDialog.cpp

HEADERS += \
        MainWindow.h \
    IssueTableWidget.h \
    UserPassDialog.h \
    TimePickerDialog.h

FORMS += \
        MainWindow.ui \
    UserPassDialog.ui \
    TimePickerDialog.ui

RESOURCES += \
    res.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../logic/ -llogic

INCLUDEPATH += $$PWD/../logic
DEPENDPATH += $$PWD/../logic

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../logic/liblogic.a

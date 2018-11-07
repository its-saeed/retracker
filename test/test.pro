include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= gui
CONFIG += core

HEADERS +=    

SOURCES +=     main.cpp \
    TimeManagerFunctionality.cpp \
    CommonsFunctionality.cpp \
    StatisticsFunctionality.cpp \
    TimerFunctionality.cpp \
    IssueManagerFunctionality.cpp

unix:!macx: LIBS += -L$$OUT_PWD/../logic/ -llogic

INCLUDEPATH += $$PWD/../logic
DEPENDPATH += $$PWD/../logic

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../logic/liblogic.a

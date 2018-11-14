include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= gui
QT += core sql

HEADERS +=

SOURCES +=     main.cpp \
    TimeManagerFunctionality.cpp \
    CommonsFunctionality.cpp \
    StatisticsFunctionality.cpp \
    TimerFunctionality.cpp \
    IssueManagerFunctionality.cpp \
    DatabaseFunctionality.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logic/release/ -llogic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logic/debug/ -llogic

INCLUDEPATH += $$PWD/../logic
DEPENDPATH += $$PWD/../logic

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/release/liblogic.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/debug/liblogic.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/release/logic.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/debug/logic.lib

LIBS += -lQt5Sql

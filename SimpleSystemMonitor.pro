QT += gui core widgets testlib

CONFIG += c++11
CONFIG -= app_bundle
LIBS += -lgtest

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    core/cpuinfo.cpp \
    core/meminfo.cpp \
    core/netinfo.cpp \
    widgets/totalwidget.cpp \
    systemmonitor.cpp \
    widgets/cpuwidget.cpp \
    widgets/emulateablewidget.cpp \
    core/unittests.cpp \
    widgets/memorywidget.cpp \
    charts/networkchart.cpp \
    charts/percenttimerealtimechart.cpp \
    charts/realtimechart.cpp \
    charts/unittest.cpp \
    utils.cpp \
    widgets/networkwidget.cpp \
    widgets/optionswidget.cpp \
    systemmonitorunittests.cpp

HEADERS += \
    core/cpuinfo.hpp \
    core/meminfo.hpp \
    core/netinfo.hpp \
    widgets/totalwidget.hpp \
    systemmonitor.hpp \
    widgets/emulateablewidget.hpp \
    widgets/cpuwidget.hpp \
    core/parseerror.hpp \
    widgets/memorywidget.hpp \
    charts/networkchart.hpp \
    charts/percenttimerealtimechart.hpp \
    charts/realtimechart.hpp \
    charts/unittest.hpp \
    utils.hpp \
    widgets/networkwidget.hpp \
    widgets/optionswidget.hpp \
    settings.hpp \
    charts/chartsnamespace.hpp \
    applicationnamespace.hpp

TRANSLATIONS += \
    translation.ts

RESOURCES += \
    resources.qrc

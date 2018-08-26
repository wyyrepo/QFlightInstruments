#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T17:03:18
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example1
TEMPLATE = app
CONFIG += c++11

#-------------------------------------------------

win32: DEFINES += WIN32 _WINDOWS _USE_MATH_DEFINES

win32:CONFIG(release, debug|release):    DEFINES += NDEBUG
else:win32:CONFIG(debug, debug|release): DEFINES += _DEBUG

#-------------------------------------------------

INCLUDEPATH += ./ ./example

#-------------------------------------------------

HEADERS += \
    example/LayoutSquare.hpp \
    example/MainWindow.hpp \
    example/WidgetADI.hpp \
    example/WidgetALT.hpp \
    example/WidgetASI.hpp \
    example/WidgetHSI.hpp \
    example/WidgetNAV.hpp \
    example/WidgetPFD.hpp \
    example/WidgetTC.hpp \
    example/WidgetVSI.hpp \
    Adi.hpp \
    Alt.hpp \
    Asi.hpp \
    Hsi.hpp \
    Nav.hpp \
    Pfd.hpp \
    Tc.hpp \
    Vsi.hpp \
    example/WidgetSix.hpp

SOURCES += \
    example/LayoutSquare.cpp \
    example/main.cpp \
    example/MainWindow.cpp \
    example/WidgetADI.cpp \
    example/WidgetALT.cpp \
    example/WidgetASI.cpp \
    example/WidgetHSI.cpp \
    example/WidgetNAV.cpp \
    example/WidgetPFD.cpp \
    example/WidgetTC.cpp \
    example/WidgetVSI.cpp \
    Adi.cpp \
    Alt.cpp \
    Asi.cpp \
    Hsi.cpp \
    Nav.cpp \
    Pfd.cpp \
    Tc.cpp \
    Vsi.cpp \
    example/WidgetSix.cpp

FORMS += \
    example/MainWindow.ui \
    example/WidgetADI.ui \
    example/WidgetALT.ui \
    example/WidgetASI.ui \
    example/WidgetHSI.ui \
    example/WidgetNAV.ui \
    example/WidgetPFD.ui \
    example/WidgetTC.ui \
    example/WidgetVSI.ui \
    example/WidgetSix.ui

RESOURCES += \
    qfi.qrc

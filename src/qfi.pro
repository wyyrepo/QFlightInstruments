#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T16:09:01
#
#-------------------------------------------------

QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qfi
TEMPLATE = lib
CONFIG += c++11

#-------------------------------------------------

DEFINES += QFI_LIBRARY

win32: DEFINES += WIN32 _WINDOWS _USE_MATH_DEFINES

win32:CONFIG(release, debug|release):    DEFINES += NDEBUG
else:win32:CONFIG(debug, debug|release): DEFINES += _DEBUG

#-------------------------------------------------

INCLUDEPATH += ./

#-------------------------------------------------

HEADERS += \
    Adi.hpp \
    Alt.hpp \
    Asi.hpp \
    Hsi.hpp \
    Nav.hpp \
    Pfd.hpp \
    Tc.hpp \
    Vsi.hpp

SOURCES += \
    Adi.cpp \
    Alt.cpp \
    Asi.cpp \
    Hsi.cpp \
    Nav.cpp \
    Pfd.cpp \
    Tc.cpp \
    Vsi.cpp

RESOURCES += \
    qfi.qrc



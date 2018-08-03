#-------------------------------------------------
# Author: Kopylov Nikita
# Github: github.com/WTFTrick/
# Bitbucket: https://bitbucket.org/Kopylov_Nick/
#
# Contacts:
# nick.kopylov@protonmail.com
# www.ves66a96@mail.ru
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MathModeling
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

#INCLUDEPATH += C:/BOOST/include/boost-1_55
#LIBS += "-LC:\BOOST\lib"

SOURCES += main.cpp\
        mathmodeling.cpp \
        binomialdistr.cpp \
        qcustomplot.cpp \
        uniformdistr.cpp \
        exponentialdistr.cpp \
    rileydistr.cpp \
    poissondistr.cpp \
    gaussdistr.cpp \
    cauchydistr.cpp \
    leastsquares.cpp \
    robustfit.cpp \
    montecarlointegration.cpp \
    celluralautomaton.cpp \
    queuinsystem.cpp

HEADERS  += mathmodeling.h \
        binomialdistr.h \
        qcustomplot.h \
        uniformdistr.h \
        exponentialdistr.h \
    rileydistr.h \
    poissondistr.h \
    gaussdistr.h \
    cauchydistr.h \
    leastsquares.h \
    robustfit.h \
    montecarlointegration.h \
    celluralautomaton.h \
    queuinsystem.h

DISTFILES += \
    resources/mathmodel.ico

win32:RC_ICONS += resources/mathmodel.ico

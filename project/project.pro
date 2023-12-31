#-------------------------------------------------
#
# Project created by QtCreator 2018-10-27T22:07:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = comp2012hpa4
TEMPLATE = app

INCLUDEPATH += ui
INCLUDEPATH += src

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
    ui/gamewindow.cpp \
    ui/square.cpp \
    src/Bishop.cpp \
    src/ChessGame.cpp \
    src/ChessPiece.cpp \
    src/King.cpp \
    src/Knight.cpp \
    src/Pawn.cpp \
    src/Queen.cpp \
    src/Rook.cpp \
    ui/promotiondialog.cpp

HEADERS += \
        ui/mainwindow.h \
    ui/gamewindow.h \
    ui/square.h \
    src/Bishop.h \
    src/ChessGame.h \
    src/ChessPiece.h \
    src/King.h \
    src/Knight.h \
    src/Pawn.h \
    src/Player.h \
    src/Queen.h \
    src/Rook.h \
    ui/promotiondialog.h

FORMS += \
        ui/mainwindow.ui \
    ui/gamewindow.ui \
    ui/promotiondialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \

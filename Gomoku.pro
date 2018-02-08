######################################################################
# Automatically generated by qmake (3.1) Fri Feb 2 19:06:25 2018
######################################################################

TEMPLATE = app
TARGET = Gomoku
INCLUDEPATH += .

QT += gui widgets network

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/gomoku.h \
    src/piece.h \
    src/board.h \
    src/const.h \
    src/server.h \
    src/connection.h \
    src/connectionthread.h \
    src/connectdialog.h
FORMS += ui/gomoku.ui \
    ui/connectdialog.ui
SOURCES += src/gomoku.cpp src/main.cpp \
    src/piece.cpp \
    src/board.cpp \
    src/server.cpp \
    src/connection.cpp \
    src/connectionthread.cpp \
    src/connectdialog.cpp
RESOURCES += res/res.qrc

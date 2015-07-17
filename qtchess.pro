#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T15:23:32
#
#-------------------------------------------------

QT       += core gui svg webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = qtchess
TEMPLATE = app
QMAKE_CXXFLAGS += -O2 -march=native

SOURCES += main.cpp\
        main-window.cpp \
    board-widget.cpp \
    settings-manager.cpp \
    game-model.cpp \
    pieces.cpp \
    move.cpp \
    common.cpp \
    board-widget-state.cpp \
    piece-set.cpp \
    promotion-dialog.cpp \
    settings-dialog.cpp \
    game-tree.cpp \
    game-tree-widget.cpp \
    notation.cpp

HEADERS  += main-window.hpp \
    board-widget.hpp \
    settings-manager.hpp \
    game-model.hpp \
    pieces.hpp \
    move.hpp \
    common.hpp \
    board-widget-state.hpp \
    piece-set.hpp \
    promotion-dialog.hpp \
    settings-dialog.hpp \
    game-tree.hpp \
    game-tree-widget.hpp \
    notation.hpp

FORMS    += main-window.ui \
    promotion-dialog.ui \
    settings-dialog.ui

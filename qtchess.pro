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
        gui/main-window.cpp \
    gui/board-widget.cpp \
    settings-manager.cpp \
    game/game-model.cpp \
    game/pieces.cpp \
    game/move.cpp \
    common.cpp \
    gui/board-widget-state.cpp \
    piece-set.cpp \
    gui/promotion-dialog.cpp \
    gui/settings-dialog.cpp \
    game/game-tree.cpp \
    gui/game-tree-widget.cpp \
    notation.cpp \
    gui/theme.cpp \
    game/player.cpp

HEADERS  += gui/main-window.hpp \
    gui/board-widget.hpp \
    settings-manager.hpp \
    game/game-model.hpp \
    game/pieces.hpp \
    game/move.hpp \
    common.hpp \
    gui/board-widget-state.hpp \
    piece-set.hpp \
    gui/promotion-dialog.hpp \
    gui/settings-dialog.hpp \
    game/game-tree.hpp \
    gui/game-tree-widget.hpp \
    notation.hpp \
    gui/theme.hpp \
    game/player.hpp

FORMS    += gui/forms/main-window.ui \
    gui/forms/promotion-dialog.ui \
    gui/forms/settings-dialog.ui

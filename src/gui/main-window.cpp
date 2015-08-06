#include "gui/main-window.hpp"
#include "ui_main-window.h"
#include "gui/promotion-dialog.hpp"
#include "gui/settings-dialog.hpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSettingsDialog(nullptr)
{
    ui->setupUi(this);
    // Setup widgets
    ui->Board->setModel(&mGameTree.getLast()->Game);

    // Connect board signals
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(onMoveMade(Move)));
    //QObject::connect(&Settings::instance(), SIGNAL(changed()), ui->Board, SLOT(update()));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), ui->Board, SLOT(flip()));
    // Connect action signals
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(onSettingsShow()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(onBoardReset()));

    // Connect text widget signals
    QObject::connect(ui->GameTextWidget, SIGNAL(positionSelected(GameTreeNode*)), this, SLOT(onPositionSet(GameTreeNode*)));
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), ui->GameTextWidget, SLOT(redraw()));
    //QObject::connect(&Settings::instance(), SIGNAL(changed()), ui->GameTextWidget, SLOT(redraw()));

    ui->GameTextWidget->setGameTree(&mGameTree);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveMade(Move move) {
    MoveType moveType = MOVE_NONSPECIAL;
    Board& board = mGameTree.getLast()->Game;

    if (board.isLegal(move, nullptr, &moveType)) {
        if (moveType == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();

            // Modify move, so that it knows about promoted piece
            move = Move(move.from(), move.to(), dialog.selectedPieceType());
        }
        onPositionSet(mGameTree.addVariation(mGameTree.getLast(), move));
    }
}

void MainWindow::onSettingsShow() {
    if (!mSettingsDialog) {
        mSettingsDialog = new SettingsDialog(this);
        QObject::connect(mSettingsDialog, SIGNAL(rejected()), this, SLOT(onSettingsClose()));
    }
    mSettingsDialog->show();
}

void MainWindow::onSettingsClose() {
    delete mSettingsDialog;
    mSettingsDialog = nullptr;
}

void MainWindow::onBoardReset() {
    mGameTree.delVariation(mGameTree.getRoot());
    ui->Board->setModel(&mGameTree.getLast()->Game);
    ui->GameTextWidget->redraw();
}

void MainWindow::onPositionSet(GameTreeNode* Node) {
    mGameTree.setLast(Node);
    ui->Board->setModel(&mGameTree.getLast()->Game);
}

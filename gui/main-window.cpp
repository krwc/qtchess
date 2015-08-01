#include "gui/main-window.hpp"
#include "ui_main-window.h"
#include "gui/promotion-dialog.hpp"
#include "gui/settings-dialog.hpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent, Settings &settings)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSettings(settings)
    , mSettingsDialog(nullptr)
{
    ui->setupUi(this);
    // Setup widgets
    ui->Board->setModel(&mGameTree.getLast()->Game);
    ui->Board->setManager(&settings);

    // Connect board signals
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(onMoveMade(Move)));
    // Connect action signals
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(onSettingsShow()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(onBoardReset()));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), this, SLOT(onBoardFlip()));
    // Connect text widget signals
    QObject::connect(ui->GameTextWidget, SIGNAL(positionSelected(GameTreeNode*)), this, SLOT(onPositionSet(GameTreeNode*)));
    ui->GameTextWidget->setGameTree(&mGameTree);

    // Calling this method to load settings at startup automatically
    onSettingsChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveMade(Move move) {
    MoveType Type = MOVE_NONSPECIAL;
    GameModel& Model = mGameTree.getLast()->Game;

    if (Model.isLegal(move, NULL, &Type)) {
        if (Type == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();
            move.PromotionPiece = dialog.getSelectedPiece();
        }
        onPositionSet(mGameTree.addVariation(mGameTree.getLast(), move));
        ui->GameTextWidget->redraw();
    }
}

void MainWindow::onSettingsShow() {
    if (!mSettingsDialog) {
        mSettingsDialog = new SettingsDialog(this, &mSettings);
        QObject::connect(mSettingsDialog, SIGNAL(rejected()), this, SLOT(onSettingsClose()));
        QObject::connect(mSettingsDialog, SIGNAL(settingsChanged()), this, SLOT(onSettingsChanged()));
    }
    mSettingsDialog->show();
}

void MainWindow::onSettingsClose() {
    delete mSettingsDialog;
    mSettingsDialog = nullptr;
}

void MainWindow::onSettingsChanged() {
    qApp->setStyleSheet(mSettings.getTheme().getSource());

    ui->Board->update();
    ui->Board->redraw();
}

void MainWindow::onBoardFlip() {
    ui->Board->flip();
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

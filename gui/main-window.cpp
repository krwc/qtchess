#include "main-window.hpp"
#include "ui_main-window.h"
#include "promotion-dialog.hpp"
#include "settings-dialog.hpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent, SettingsManager &Manager)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mManager(Manager)
    , mSettings(nullptr)
{
    ui->setupUi(this);
    ui->Board->setModel(&mGameTree.getLast()->Game);
    ui->Board->setManager(&mManager);
    ui->GameTextWidget->setGameTree(&mGameTree);
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(moveMade(Move)));
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this,
                     SLOT(showSettings()));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), this, SLOT(flipBoard()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(resetBoard()));
    QObject::connect(ui->GameTextWidget, SIGNAL(positionSelected(GameTreeNode*)),
                     this, SLOT(setPosition(GameTreeNode*)));
    settingsChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveMade(Move move) {
    MoveType Type = MOVE_NONSPECIAL;
    GameModel& Model = mGameTree.getLast()->Game;

    if (Model.isLegal(move, NULL, &Type)) {
        if (Type == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();
            move.PromotionPiece = dialog.getSelectedPiece();
        }
        setPosition(mGameTree.addVariation(mGameTree.getLast(), move));
        ui->GameTextWidget->redraw();
    }
}

void MainWindow::showSettings() {
    if (!mSettings) {
        mSettings = new SettingsDialog(this, &mManager);
        QObject::connect(mSettings, SIGNAL(rejected()), this, SLOT(closeSettings()));
        QObject::connect(mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    }
    mSettings->show();
}

void MainWindow::closeSettings() {
    delete mSettings;
    mSettings = nullptr;
}

void MainWindow::settingsChanged() {
    qApp->setStyleSheet(mManager.getTheme().getSource());

    ui->Board->update();
    ui->Board->redraw();
}

void MainWindow::flipBoard() {
    ui->Board->flip();
}

void MainWindow::resetBoard() {
    mGameTree.delVariation(mGameTree.getRoot());
    ui->Board->setModel(&mGameTree.getLast()->Game);
    ui->GameTextWidget->redraw();
}

void MainWindow::setPosition(GameTreeNode* Node) {
    mGameTree.setLast(Node);
    ui->Board->setModel(&mGameTree.getLast()->Game);
}

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
    ui->Board->setModel(&mModel);
    ui->Board->setManager(&mManager);
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(moveMade(Move)));
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), this, SLOT(flipBoard()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveMade(Move move) {
    MoveType Type = MOVE_NONSPECIAL;
    if (mModel.isLegal(move, NULL, &Type)) {
        if (Type == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();
            move.PromotionPiece = dialog.getSelectedPiece();
        }
        mModel.makeMove(move);
        ui->GameTextWidget->addMove(move);
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
    ui->Board->update();
    ui->Board->redraw();
}

void MainWindow::flipBoard() {
    ui->Board->flip();
}

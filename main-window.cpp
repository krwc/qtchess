#include "main-window.hpp"
#include "ui_main-window.h"
#include "promotion-dialog.hpp"

MainWindow::MainWindow(QWidget *parent, SettingsManager &Manager)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mManager(Manager)
{
    ui->setupUi(this);
    ui->Board->setModel(&mModel);
    ui->Board->setManager(&mManager);
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(moveMade(Move)));
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
    }
}

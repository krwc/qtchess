#include "gui/main-window.hpp"
#include "ui_main-window.h"
#include "gui/promotion-dialog.hpp"
#include "gui/settings/settings-dialog.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSettingsDialog(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("QtChess");
    // Setup widgets
    ui->Board->setModel(&mTree.currentNode()->board());
    ui->GameTextWidget->setTree(&mTree);

    // Connect board signals
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(onMoveMade(Move)));
    //QObject::connect(&Settings::instance(), SIGNAL(changed()), ui->Board, SLOT(update()));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), ui->Board, SLOT(flip()));
    // Connect action signals
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(onSettingsShow()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(onBoardReset()));

    // Connect text widget signals
    QObject::connect(ui->GameTextWidget, &MoveTreeWidget::moveSelected, this, &MainWindow::onPositionSet);

    QObject::connect(&mTree, &Tree::changed, this, &MainWindow::onPositionChanged);
    // XXX: Watch out, this can be a potential race condition if Tree is modified by other thread.
    QObject::connect(&mTree, &Tree::changed, ui->GameTextWidget, &MoveTreeWidget::redraw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveMade(Move move) {
    MoveType moveType = MOVE_NONSPECIAL;
    const Board& board = mTree.currentNode()->board();

    if (board.isLegal(move, nullptr, &moveType)) {
        if (moveType == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();

            // Modify move, so that it knows about promoted piece
            move = Move(move.from(), move.to(), dialog.selectedPieceType());
        }
        mTree.addMove(move);
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

void MainWindow::onBoardReset()
{
    mTree.clear();
}

void MainWindow::onPositionChanged()
{
    ui->Board->setModel(&mTree.currentNode()->board());
    ui->engineWidget->setBoard(mTree.currentNode()->board());
}

void MainWindow::onPositionSet(size_t uid)
{
    mTree.setCurrent(TreeNode::fromUid(uid));
}

#include "gui/main-window.hpp"
#include "ui_main-window.h"
#include "gui/promotion-dialog.hpp"
#include "gui/settings/settings-dialog.hpp"
#include "gui/settings/engine-settings-dialog.hpp"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settingsDialog(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("QtChess");
    // Setup widgets
    ui->Board->setModel(&m_tree.currentNode()->board());
    ui->GameTextWidget->setTree(&m_tree);

    // Connect board signals
    QObject::connect(ui->Board, SIGNAL(moveMade(Move)), this, SLOT(onMoveMade(Move)));
    QObject::connect(ui->actionFlip, SIGNAL(triggered()), ui->Board, SLOT(flip()));
    // Connect action signals
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(onSettingsShow()));
    QObject::connect(ui->actionEngines, SIGNAL(triggered()), this, SLOT(onEnginesShow()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(onBoardReset()));
    QObject::connect(ui->actionSetFen, SIGNAL(triggered()), this, SLOT(onSetFen()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    // Connect text widget signals
    QObject::connect(ui->GameTextWidget, &MoveTreeWidget::moveSelected, this, &MainWindow::onPositionSet);

    QObject::connect(&m_tree, &Tree::changed, this, &MainWindow::onPositionChanged);
    // XXX: Watch out, this can be a potential race condition if Tree is modified by other thread.
    QObject::connect(&m_tree, &Tree::changed, ui->GameTextWidget, &MoveTreeWidget::redraw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveMade(Move move) {
    MoveType moveType = MOVE_NONSPECIAL;
    const Board& board = m_tree.currentNode()->board();

    if (board.isLegal(move, nullptr, &moveType)) {
        if (moveType == MOVE_PROMOTION) {
            PromotionDialog dialog(this);
            dialog.exec();

            // Modify move, so that it knows about promoted piece
            move = Move(move.from(), move.to(), dialog.selectedPieceType());
        }
        m_tree.addMove(move);
    }
}

void MainWindow::onSettingsShow() {
    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
        QObject::connect(m_settingsDialog, SIGNAL(rejected()), this, SLOT(onSettingsClose()));
    }
    m_settingsDialog->show();
}

void MainWindow::onSettingsClose() {
    delete m_settingsDialog;
    m_settingsDialog = nullptr;
}

void MainWindow::onEnginesShow()
{
    QScopedPointer<EngineSettingsDialog>(new EngineSettingsDialog(this))->exec();
}

void MainWindow::onBoardReset()
{
    m_tree.clear();
}

void MainWindow::onPositionChanged()
{
    ui->Board->setModel(&m_tree.currentNode()->board());
    ui->engineWidget->setBoard(m_tree.currentNode()->board());
}

void MainWindow::onPositionSet(size_t uid)
{
    m_tree.setCurrent(TreeNode::fromUid(uid));
}

void MainWindow::onSetFen()
{
    Board board;
    QInputDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        // Invalid FEN.
        if (!board.setFen(dialog.textValue())) {
            QMessageBox::information(this, "Fen string invalid",
                                     tr("Passed string '%1' is not a valid FEN string").arg(dialog.textValue()));
        } else
            m_tree.setRootBoard(board);
    }
}

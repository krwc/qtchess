#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "settings.hpp"
#include "settings-dialog.hpp"
#include "game/game-tree.hpp"
#include <QMainWindow>

struct GameTreeNode;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent);
    ~MainWindow();
private slots:
    void onMoveMade(Move move);
    void onSettingsShow();
    void onSettingsClose();
    void onBoardReset();

    void onPositionSet(GameTreeNode*);
private:
    Ui::MainWindow *ui;
    // Game tree
    GameTree mGameTree;
    // Settings dialog
    SettingsDialog* mSettingsDialog;

};

#endif // MAINWINDOW_HPP

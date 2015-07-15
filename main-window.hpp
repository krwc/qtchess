#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "settings-manager.hpp"
#include "settings-dialog.hpp"
#include "game-tree.hpp"
#include <QMainWindow>

struct GameTreeNode;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, SettingsManager &Manager);
    ~MainWindow();
private slots:
    void moveMade(Move move);
    void showSettings();
    void closeSettings();
    void settingsChanged();
    void flipBoard();

    void setPosition(GameTreeNode*);
private:
    Ui::MainWindow *ui;
    // Settings manager
    SettingsManager& mManager;
    // Game tree
    GameTree mGameTree;
    // Settings dialog
    SettingsDialog* mSettings;

};

#endif // MAINWINDOW_HPP

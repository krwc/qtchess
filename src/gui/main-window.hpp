#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP
#include "gui/settings/settings-dialog.hpp"
#include "game/tree.hpp"
#include <QMainWindow>

class TreeNode;

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
    void onEnginesShow();
    void onSettingsClose();
    void onBoardReset();
    void onPositionChanged();
    void onPositionSet(size_t);
private:
    Ui::MainWindow *ui;
    // Game tree
    Tree mTree;
    // Settings dialog
    SettingsDialog* mSettingsDialog;

};

#endif // MAIN_WINDOW_HPP

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP
#include "gui/settings/settings-dialog.hpp"
#include "game/tree.hpp"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class TreeNode;
class MainWindow : public QMainWindow {
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
    void onSetFen();
    void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
    // Game tree
    Tree m_tree;
    // Settings dialog
    SettingsDialog* m_settingsDialog;

};

#endif // MAIN_WINDOW_HPP

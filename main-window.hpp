#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "settings-manager.hpp"
#include "game-model.hpp"
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, SettingsManager &Manager);
    ~MainWindow();
public slots:
    void moveMade(Move move);
private:
    Ui::MainWindow *ui;
    // Settings manager
    SettingsManager& mManager;
    // Game model
    GameModel mModel;
};

#endif // MAINWINDOW_HPP

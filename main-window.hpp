#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "settings-manager.hpp"
#include "settings-dialog.hpp"
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
    void showSettings();
    void closeSettings();
    void settingsChanged();
private:
    Ui::MainWindow *ui;
    // Settings manager
    SettingsManager& mManager;
    // Game model
    GameModel mModel;
    // Settings dialog
    SettingsDialog* mSettings;

};

#endif // MAINWINDOW_HPP

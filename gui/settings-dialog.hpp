#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

class SettingsManager;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, SettingsManager* Manager);
    ~SettingsDialog();
private slots:
    void lightSquaresColorClicked();
    void darkSquaresColorClicked();
    void selectionColorClicked();
    void checkBoxToggled(bool checked);
    void pieceSetChanged(const QString&);
    void saveClicked();
    void resetClicked();
signals:
    void settingsChanged();
private:
    Ui::SettingsDialog *ui;

    SettingsManager* mManager;
    // Reads settings and updates widgets to represent current settings state.
    void readSettings();
};

#endif // SETTINGSDIALOG_HPP

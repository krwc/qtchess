#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent);
    ~SettingsDialog();
private slots:
    void onLsColorChanged(QColor);
    void onDsColorChanged(QColor);
    void onSelectionColorChanged(QColor);

    void checkBoxToggled(bool checked);
    void pieceSetChanged(const QString&);
    void saveClicked();
    void resetClicked();
private:
    Ui::SettingsDialog *ui;
    // Reads settings and updates widgets to represent current settings state.
    void readSettings();
};

#endif // SETTINGSDIALOG_HPP

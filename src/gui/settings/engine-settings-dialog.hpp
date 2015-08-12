#ifndef ENGINE_SETTINGS_DIALOG_HPP
#define ENGINE_SETTINGS_DIALOG_HPP
#include <QDialog>

namespace Ui {
class EngineSettingsDialog;
}

class EngineSettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineSettingsDialog(QWidget* parent = 0);
    ~EngineSettingsDialog();
private slots:
    void onRemoveClicked();
    void onCreateClicked();
    void onEditClicked();
private:
    Ui::EngineSettingsDialog *ui;
};

#endif // ENGINE_SETTINGS_DIALOG_HPP

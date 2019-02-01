#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP
#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class ColorButton;
class QCheckBox;
class QLineEdit;
class AbstractSettings;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent);
    ~SettingsDialog();
private slots:
    void pieceSetChanged(const QString& value);
    void saveClicked();
    void resetClicked();
private:
    Ui::SettingsDialog *ui;

    void mapWithSetting(AbstractSettings&, QString key, ColorButton* button);
    void mapWithSetting(AbstractSettings&, QString key, QCheckBox* box);
    void mapWithSetting(AbstractSettings&, QString key, QLineEdit* line);

    /*! \brief Reads settings and updates widgets to represent current settings state. */
    void readSettings();
};

#endif // SETTINGS_DIALOG_HPP

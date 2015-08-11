#ifndef ENGINE_OPTIONS_DIALOG_HPP
#define ENGINE_OPTIONS_DIALOG_HPP
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include "engine/engine.hpp"

namespace Ui {
class EngineOptionsDialog;
}

class AbstractSettings;
class EngineSettings;
class EngineOptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineOptionsDialog(EngineSettings& settings, QWidget* parent = 0);
    ~EngineOptionsDialog();
private slots:
    void onOptionsParsed(QList<EngineOption>);
    void onSave();
private:
    QLineEdit* createLineEdit(QString key, EngineOption option);
    QCheckBox* createCheckBox(QString key, EngineOption option);
    QSpinBox* createSpinBox(QString key, EngineOption option);
    Ui::EngineOptionsDialog *ui;
    EngineSettings& m_settings;
    Engine m_engine;
};

#endif // ENGINE_OPTIONS_DIALOG_HPP

#ifndef ENGINE_EDIT_OPTIONS_DIALOG_HPP
#define ENGINE_EDIT_OPTIONS_DIALOG_HPP
#include <QDialog>

#include "engine/engine-option.hpp"
#include "engine/engine.hpp"

namespace Ui {
class EngineEditOptionsDialog;
}

class QLineEdit;
class QCheckBox;
class QSpinBox;

class EngineEditOptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineEditOptionsDialog(EngineConfig config, QWidget* parent = 0);
    ~EngineEditOptionsDialog();

    /*! \brief Returns modified engine config, ready to save */
    EngineConfig engineConfig() const {
        return m_config;
    }
private slots:
    void onOkClicked();
//    void onDefaultsClicked();
private:
    void rebuildTable();
    QLineEdit* createLineEdit(QString key, EngineOption option);
    QCheckBox* createCheckBox(QString key, EngineOption option);
    QSpinBox* createSpinBox(QString key, EngineOption option);

    Ui::EngineEditOptionsDialog *ui;
    EngineConfig m_config;
};

#endif // ENGINE_EDIT_OPTIONS_DIALOG_HPP

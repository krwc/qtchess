#ifndef ENGINE_EDIT_DIALOG_HPP
#define ENGINE_EDIT_DIALOG_HPP
#include <QDialog>
#include "engine/engine.hpp"
#include "settings/settings-factory.hpp"

namespace Ui {
class EngineEditDialog;
}

class QTableWidget;

class EngineEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineEditDialog(EngineConfig config = EngineConfig(), QWidget* parent = 0);
    ~EngineEditDialog();

    /*! \brief Engine config after in-dialog modifications.
     * It is valid if and only if dialog emited \a accept().
     */
    EngineConfig engineConfig() const;
private slots:
    void onOkClicked();
    void onCancelClicked();
    void onConfigureClicked();
private:
    bool settingsValid();

    Ui::EngineEditDialog *ui;
    EngineConfig m_config;
    QStringList m_registeredEngines;
};

#endif // ENGINE_EDIT_DIALOG_HPP

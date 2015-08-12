#ifndef ENGINE_SELECTION_DIALOG_HPP
#define ENGINE_SELECTION_DIALOG_HPP
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "engine/engine-config.hpp"
#include "gui/engine-list-widget.hpp"

class EngineSelectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineSelectionDialog(QWidget* parent = nullptr);

    QString engineName() const {
        return m_engineName;
    }
private slots:
    void onDoubleClicked();
private:
    EngineListWidget* m_list;
    QPushButton* m_cancelButton;
    QString m_engineName;
};

#endif // ENGINE_SELECTION_DIALOG_HPP

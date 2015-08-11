#ifndef ENGINE_SELECTION_DIALOG_HPP
#define ENGINE_SELECTION_DIALOG_HPP
#include <QDialog>
#include <QListWidget>
#include <QPushButton>

class EngineSelectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineSelectionDialog(QWidget* parent = nullptr);

    QString selectedEngine() const {
        return m_selectedEngine;
    }
private slots:
    void onDoubleClicked(QListWidgetItem*);
private:
    QListWidget* m_listWidget;
    QPushButton* m_cancelButton;
    QString m_selectedEngine;
};

#endif // ENGINE_SELECTION_DIALOG_HPP

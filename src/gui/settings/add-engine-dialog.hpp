#ifndef ADD_ENGINE_DIALOG_HPP
#define ADD_ENGINE_DIALOG_HPP
#include <QDialog>

namespace Ui {
class AddEngineDialog;
}

class AddEngineDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddEngineDialog(QWidget* parent = 0);
    ~AddEngineDialog();

    QString name() const {
        return m_name;
    }

    QString path() const {
        return m_path;
    }

    QString workdir() const {
        return m_workdir;
    }

    QString arguments() const {
        return m_arguments;
    }
private slots:
    void onAddClicked();
private:
    Ui::AddEngineDialog *ui;
    QString m_name;
    QString m_path;
    QString m_workdir;
    QString m_arguments;
};


#endif // ADD_ENGINE_DIALOG_HPP

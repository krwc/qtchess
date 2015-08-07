#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QVector>
#include "settings.hpp"

namespace Ui {
class SettingsDialog;
}

class ColorButton;
class QCheckBox;

/*! \brief internal setting entry class to simply settings <-> widgets mapping */
class AbstractEntry : public QObject {
    Q_OBJECT
public:
    /*! \brief constructs entry for given key */
    AbstractEntry(Settings::Key key);
    /*! \brief returns key */
    Settings::Key key() const;
    /*! \brief allows resetting widget representation of the property */
    virtual void reset() = 0;
public slots:
    virtual void onSetValue(const QVariant&);
private:
    Settings::Key m_key;
};

class ColorEntry : public AbstractEntry {
    Q_OBJECT
public:
    ColorEntry(ColorButton* button, Settings::Key key);
    virtual void reset();
private:
    ColorButton* m_button;
};

class CheckBoxEntry : public AbstractEntry {
    Q_OBJECT
public:
    CheckBoxEntry(QCheckBox* box, Settings::Key key);
    virtual void reset();
private:
    QCheckBox* m_box;
};


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent);
    ~SettingsDialog();
private slots:
    void pieceSetChanged(const QString&);
    void saveClicked();
    void resetClicked();
private:
    Ui::SettingsDialog *ui;
    /*! \brief list of defined and displayed entries */
    QVector<AbstractEntry*> m_entries;
    // Reads settings and updates widgets to represent current settings state.
    void readSettings();
};

#endif // SETTINGSDIALOG_HPP

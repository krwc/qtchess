#include "settings-dialog.hpp"
#include "settings.hpp"
#include "piece-set.hpp"
#include "ui_settings-dialog.h"
#include <QColorDialog>
#include <QColor>

AbstractEntry::AbstractEntry(Settings::Key key)
    : m_key(key)
{
}

Settings::Key AbstractEntry::key() const
{
    return m_key;
}

void AbstractEntry::onSetValue(const QVariant& value)
{
    Settings::instance().set(key(), value);
}



ColorEntry::ColorEntry(ColorButton *button, Settings::Key key)
    : AbstractEntry(key)
    , m_button(button)
{
    QObject::connect(m_button, &ColorButton::changed, this, &AbstractEntry::onSetValue);
}

void ColorEntry::reset()
{
    m_button->setColor(Settings::instance().get(key()).value<QColor>());
}


CheckBoxEntry::CheckBoxEntry(QCheckBox* box, Settings::Key key)
    : AbstractEntry(key)
    , m_box(box)
{
    QObject::connect(m_box, &QCheckBox::toggled, this, &AbstractEntry::onSetValue);
}

void CheckBoxEntry::reset()
{
    m_box->setChecked(Settings::instance().get(key()).toBool());
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Settings");

    m_entries.push_back(new ColorEntry(ui->lightSqColor, Settings::LightSquareColor));
    m_entries.push_back(new ColorEntry(ui->darkSqColor, Settings::DarkSquareColor));
    m_entries.push_back(new ColorEntry(ui->selectionColor, Settings::SelectionColor));
    m_entries.push_back(new ColorEntry(ui->pgnBgColor, Settings::PgnBackgroundColor));
    m_entries.push_back(new ColorEntry(ui->pgnNoMoveColor, Settings::PgnMoveColor));
    m_entries.push_back(new ColorEntry(ui->pgnHiMoveColor, Settings::PgnHiMoveColor));
    m_entries.push_back(new ColorEntry(ui->pgnHiColor, Settings::PgnHiColor));
    m_entries.push_back(new ColorEntry(ui->pgnNumberColor, Settings::PgnNumberColor));
    m_entries.push_back(new ColorEntry(ui->pgnVariationColor, Settings::PgnVariationColor));
    m_entries.push_back(new ColorEntry(ui->coordsBorderColor, Settings::CoordsBorderColor));
    m_entries.push_back(new ColorEntry(ui->coordsTextColor, Settings::CoordsTextColor));
    m_entries.push_back(new CheckBoxEntry(ui->coordsCheckBox, Settings::ShouldDrawCoords));

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    Settings::instance().reset();

    for (AbstractEntry* entry : m_entries)
        delete entry;
    delete ui;
}

void SettingsDialog::saveClicked() {
    Settings::instance().save();
    close();
}

void SettingsDialog::resetClicked() {
    // Reset and re-read
    Settings::instance().reset();
    readSettings();
}

void SettingsDialog::pieceSetChanged(const QString& Value)
{
    if (Value == "")
        return;

    Settings::instance().set(Settings::PieceStyleName, Value);
}

void SettingsDialog::readSettings() {
    ui->PieceSetList->clear();

    for (auto& entry : m_entries)
        entry->reset();

    QStringList PiecesList = PieceSet::getAvailableSets();
    PiecesList.swap(0, PiecesList.indexOf(Settings::instance().get(Settings::PieceStyleName).toString()));
    ui->PieceSetList->addItems(PiecesList);
}

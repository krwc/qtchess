#include "gui/settings/settings-dialog.hpp"
#include "settings/settings-factory.hpp"
#include "piece-set.hpp"
#include "ui_settings-dialog.h"


// Useful macro for mapWithSetting() method.
#define SETTER_FUNCTOR(type,settings) [&, key](type value) { (settings).set(key, value); }

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Settings");

    BoardSettings& board = SettingsFactory::board();
    HtmlSettings& html = SettingsFactory::html();

    mapWithSetting(board, "colorSquareLight", ui->lightSqColor);
    mapWithSetting(board, "colorSquareDark", ui->darkSqColor);
    mapWithSetting(board, "colorPicking", ui->selectionColor);
    mapWithSetting(board, "colorBorder", ui->coordsBorderColor);
    mapWithSetting(board, "colorBorderText", ui->coordsTextColor);
    mapWithSetting(board, "boolBorder", ui->coordsCheckBox);

    mapWithSetting(html, "colorBackground", ui->pgnBgColor);
    mapWithSetting(html, "colorHighlight", ui->pgnHiColor);
    mapWithSetting(html, "colorMove", ui->pgnNoMoveColor);
    mapWithSetting(html, "colorMoveNumber", ui->pgnNumberColor);
    mapWithSetting(html, "colorMoveHighlight", ui->pgnHiMoveColor);
    mapWithSetting(html, "colorVariant", ui->pgnVariationColor);

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    // Restore saved settings.
    SettingsFactory::board().reset();
    SettingsFactory::html().reset();

    delete ui;
}

void SettingsDialog::saveClicked() {
    SettingsFactory::board().save();
    SettingsFactory::html().save();
    close();
}

void SettingsDialog::resetClicked() {
    // Reset and re-read
    SettingsFactory::board().reset();
    SettingsFactory::html().reset();

    readSettings();
}

void SettingsDialog::pieceSetChanged(const QString& value)
{
    if (value == "")
        return;

    SettingsFactory::board().set("stringPieceStyle", value);
}

void SettingsDialog::mapWithSetting(AbstractSettings& settings, QString key,
                                    ColorButton* button)
{
    button->setColor(settings.get(key).value<QColor>());
    QObject::connect(button, &ColorButton::changed, SETTER_FUNCTOR(QColor, settings));
}

void SettingsDialog::mapWithSetting(AbstractSettings& settings, QString key,
                                    QCheckBox* box)
{
    box->setChecked(settings.get(key).toBool());
    QObject::connect(box, &QCheckBox::clicked, SETTER_FUNCTOR(bool, settings));
}

void SettingsDialog::readSettings()
{
    QComboBox* comboPieces = ui->PieceSetList;
    QStringList pieces = PieceSet::getAvailableSets();
    pieces.swap(0, pieces.indexOf(SettingsFactory::board().get("stringPieceStyle").toString()));

    comboPieces->clear();
    comboPieces->addItems(pieces);
}

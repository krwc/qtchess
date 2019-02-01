#include "gui/settings/settings-dialog.hpp"
#include "settings/settings-factory.hpp"
#include "piece-set.hpp"
#include "ui_settings-dialog.h"


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

    const auto minFontScaling = 1.0;
    const auto maxFontScaling = 10.0;
    const auto decimalDigits = 2;
    ui->pgnFontScaling->setValidator(
        new QDoubleValidator(minFontScaling, maxFontScaling, decimalDigits));
    mapWithSetting(html, "fontScaling", ui->pgnFontScaling);
    mapWithSetting(html, "colorBackground", ui->pgnBgColor);
    mapWithSetting(html, "colorHighlight", ui->pgnHiColor);
    mapWithSetting(html, "colorMove", ui->pgnNoMoveColor);
    mapWithSetting(html, "colorMoveNumber", ui->pgnNumberColor);
    mapWithSetting(html, "colorMoveHighlight", ui->pgnHiMoveColor);
    mapWithSetting(html, "colorVariant", ui->pgnVariationColor);
    mapWithSetting(html, "colorAnnotation", ui->pgnAnnotationColor);

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
    QObject::connect(button, &ColorButton::changed,
                     [&, key](auto value) { settings.set(key, value); });
}

void SettingsDialog::mapWithSetting(AbstractSettings& settings, QString key,
                                    QCheckBox* box)
{
    box->setChecked(settings.get(key).toBool());
    QObject::connect(box, &QCheckBox::clicked,
                     [&, key](auto value) { settings.set(key, value); });
}

void SettingsDialog::mapWithSetting(AbstractSettings& settings, QString key,
                                    QLineEdit* line)
{
    line->setText(settings.get(key).toString());
    QObject::connect(line, &QLineEdit::editingFinished,
                     [&, key, line]() { settings.set(key, line->text()); });
}

void SettingsDialog::readSettings()
{
    QComboBox* comboPieces = ui->PieceSetList;
    QStringList pieces = PieceSet::getAvailableSets();
    pieces.swap(0, pieces.indexOf(SettingsFactory::board().get("stringPieceStyle").toString()));

    comboPieces->clear();
    comboPieces->addItems(pieces);
}

#include "settings-dialog.hpp"
#include "settings.hpp"
#include "piece-set.hpp"
#include "ui_settings-dialog.h"
#include <QColorDialog>

static void SetColor(QPushButton* Button, QColor Color) {
    Button->setStyleSheet("background-color: " + Color.name());
}

static bool SelectColor(QWidget* Parent, QColor& SelectedColor) {
    QColorDialog Dialog(Parent);
    if (Dialog.exec() == QDialog::Rejected)
        return false;
    SelectedColor = Dialog.selectedColor();
    return true;
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::lightSquaresColorClicked() {
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->LSColorButton, Ret);
        Settings::instance().instance().set(Settings::LightSquareColor, Ret);
    }
}

void SettingsDialog::darkSquaresColorClicked() {
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->DSColorButton, Ret);
        Settings::instance().set(Settings::DarkSquareColor, Ret);
    }
}

void SettingsDialog::selectionColorClicked() {
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->SelectionColorButton, Ret);
        Settings::instance().set(Settings::SelectionColor, Ret);
    }
}

void SettingsDialog::checkBoxToggled(bool checked) {
    Settings::instance().set(Settings::ShouldDrawCoords, checked);
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
    SetColor(ui->LSColorButton, Settings::instance().get(Settings::LightSquareColor).value<QColor>());
    SetColor(ui->DSColorButton, Settings::instance().get(Settings::DarkSquareColor).value<QColor>());
    SetColor(ui->SelectionColorButton, Settings::instance().get(Settings::SelectionColor).value<QColor>());
    ui->CoordsCheckBox->setChecked(Settings::instance().get(Settings::ShouldDrawCoords).toBool());

    QStringList PiecesList = PieceSet::getAvailableSets();
    PiecesList.swap(0, PiecesList.indexOf(Settings::instance().get(Settings::PieceStyleName).toString()));
    ui->PieceSetList->addItems(PiecesList);

    /*QStringList ThemesList = Theme::getAvailableThemes();
    ThemesList.swap(0, ThemesList.indexOf(Settings::instance().get(Settings::instance().ThemeNamegetThemeName()));
    ui->ThemeList->addItems(ThemesList);*/
}

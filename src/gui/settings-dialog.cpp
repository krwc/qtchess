#include "settings-dialog.hpp"
#include "settings.hpp"
#include "piece-set.hpp"
#include "ui_settings-dialog.h"
#include <QColorDialog>
#include <QColor>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->lightSqColor, SIGNAL(changed(QColor)), this,
                     SLOT(onLsColorChanged(QColor)));
    QObject::connect(ui->darkSqColor, SIGNAL(changed(QColor)), this,
                     SLOT(onDsColorChanged(QColor)));
    QObject::connect(ui->selectionColor, SIGNAL(changed(QColor)), this,
                     SLOT(onSelectionColorChanged(QColor)));

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onLsColorChanged(QColor color)
{
    Settings::instance().set(Settings::LightSquareColor, color);
}

void SettingsDialog::onDsColorChanged(QColor color)
{
    Settings::instance().set(Settings::DarkSquareColor, color);
}

void SettingsDialog::onSelectionColorChanged(QColor color)
{
    Settings::instance().set(Settings::SelectionColor, color);
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

    ui->lightSqColor->setColor(Settings::instance().get(Settings::LightSquareColor).value<QColor>());
    ui->darkSqColor->setColor(Settings::instance().get(Settings::DarkSquareColor).value<QColor>());
    ui->selectionColor->setColor(Settings::instance().get(Settings::SelectionColor).value<QColor>());

    ui->CoordsCheckBox->setChecked(Settings::instance().get(Settings::ShouldDrawCoords).toBool());

    QStringList PiecesList = PieceSet::getAvailableSets();
    PiecesList.swap(0, PiecesList.indexOf(Settings::instance().get(Settings::PieceStyleName).toString()));
    ui->PieceSetList->addItems(PiecesList);
}

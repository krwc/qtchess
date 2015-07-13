#include "settings-dialog.hpp"
#include "settings-manager.hpp"
#include "ui_settingsdialog.h"
#include <QColorDialog>
#include <iostream>

static void SetColor(QPushButton* Button, QColor Color) {
    Button->setStyleSheet("background-color: " + Color.name());
}

SettingsDialog::SettingsDialog(QWidget *parent, SettingsManager* Manager)
    : QDialog(parent)
    , mManager(Manager)
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
    QColorDialog Dialog(this);
    Dialog.exec();
    SetColor(ui->LSColorButton, Dialog.selectedColor());
    mManager->setLSColor(Dialog.selectedColor());
    emit settingsChanged();
}

void SettingsDialog::darkSquaresColorClicked() {
    QColorDialog Dialog(this);
    Dialog.exec();
    SetColor(ui->DSColorButton, Dialog.selectedColor());
    mManager->setDSColor(Dialog.selectedColor());
    emit settingsChanged();
}

void SettingsDialog::selectionColorClicked() {
    QColorDialog Dialog(this);
    Dialog.exec();
    SetColor(ui->SelectionColorButton, Dialog.selectedColor());
    mManager->setSelectionColor(Dialog.selectedColor());
    emit settingsChanged();
}

void SettingsDialog::checkBoxToggled(bool checked) {
    mManager->setShouldDrawCoords(checked);
    emit settingsChanged();
}

void SettingsDialog::saveClicked() {
    close();
}

void SettingsDialog::resetClicked() {
    // Reset and re-read
    mManager->reset();
    readSettings();
}

void SettingsDialog::readSettings() {
    SetColor(ui->LSColorButton, mManager->getLSColor());
    SetColor(ui->DSColorButton, mManager->getDSColor());
    SetColor(ui->SelectionColorButton, mManager->getSelectionColor());
    ui->CoordsCheckBox->setChecked(mManager->getShouldDrawCoords());
    emit settingsChanged();
}

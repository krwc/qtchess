#include "settings-dialog.hpp"
#include "settings-manager.hpp"
#include "ui_settingsdialog.h"
#include <QColorDialog>
#include <QDebug>
#include <iostream>

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
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->LSColorButton, Ret);
        mManager->setLSColor(Ret);
    }
    emit settingsChanged();
}

void SettingsDialog::darkSquaresColorClicked() {
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->DSColorButton, Ret);
        mManager->setDSColor(Ret);
    }
    emit settingsChanged();
}

void SettingsDialog::selectionColorClicked() {
    QColor Ret;
    if (SelectColor(this, Ret)) {
        SetColor(ui->SelectionColorButton, Ret);
        mManager->setSelectionColor(Ret);
    }
    emit settingsChanged();
}

void SettingsDialog::checkBoxToggled(bool checked) {
    mManager->setShouldDrawCoords(checked);
    emit settingsChanged();
}

void SettingsDialog::saveClicked() {
    mManager->save();
    close();
}

void SettingsDialog::resetClicked() {
    // Reset and re-read
    mManager->reset();
    readSettings();
}

void SettingsDialog::pieceSetChanged(const QString& Value)
{
    if (Value == "")
        return;

    mManager->setPieceStyleName(Value);
    emit settingsChanged();
}

void SettingsDialog::readSettings() {
    ui->PieceSetList->clear();
    SetColor(ui->LSColorButton, mManager->getLSColor());
    SetColor(ui->DSColorButton, mManager->getDSColor());
    SetColor(ui->SelectionColorButton, mManager->getSelectionColor());
    ui->CoordsCheckBox->setChecked(mManager->getShouldDrawCoords());

    QStringList PiecesList = PieceSet::getAvailableSets();
    PiecesList.removeOne(mManager->getPieceStyleName());
    PiecesList.push_front(mManager->getPieceStyleName());
    ui->PieceSetList->addItems(PieceSet::getAvailableSets());

    emit settingsChanged();
}

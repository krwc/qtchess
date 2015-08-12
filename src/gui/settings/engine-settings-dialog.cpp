#include "engine-settings-dialog.hpp"
#include "ui_engine-settings-dialog.h"

#include "settings/settings-factory.hpp"
#include "gui/settings/engine-edit-dialog.hpp"

EngineSettingsDialog::EngineSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EngineSettingsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
    QObject::connect(ui->create, SIGNAL(clicked(bool)), this, SLOT(onCreateClicked()));
    QObject::connect(ui->edit, SIGNAL(clicked(bool)), this, SLOT(onEditClicked()));
    QObject::connect(ui->ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
}

EngineSettingsDialog::~EngineSettingsDialog()
{
    delete ui;
}

void EngineSettingsDialog::onRemoveClicked()
{
    if (!ui->list->selected())
        return;

    SettingsFactory::engines().remove(ui->list->engineName());
}

void EngineSettingsDialog::onCreateClicked()
{
    QScopedPointer<EngineEditDialog> dialog(new EngineEditDialog(EngineConfig(), this));

    if (dialog->exec() == QDialog::Accepted)
        SettingsFactory::engines().saveConfig(dialog->engineConfig());
}

void EngineSettingsDialog::onEditClicked()
{
    if (!ui->list->selected())
        return;

    EngineConfig config = SettingsFactory::engines().config(ui->list->engineName());
    QScopedPointer<EngineEditDialog> dialog(new EngineEditDialog(config, this));

    if (dialog->exec() == QDialog::Accepted)
        SettingsFactory::engines().saveConfig(dialog->engineConfig());
}

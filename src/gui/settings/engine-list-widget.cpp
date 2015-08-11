#include "gui/settings/add-engine-dialog.hpp"
#include "gui/settings/engine-list-widget.hpp"
#include "gui/settings/engine-options-dialog.hpp"
#include "settings/settings-factory.hpp"
#include "ui_engine-list-widget.h"


EngineListWidget::EngineListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EngineListWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->add, &QPushButton::clicked, this, &EngineListWidget::onAdd);
    QObject::connect(ui->remove, &QPushButton::clicked, this, &EngineListWidget::onRemove);
    QObject::connect(ui->configure, &QPushButton::clicked, this, &EngineListWidget::onConfigure);

    update();
}

EngineListWidget::~EngineListWidget()
{
    delete ui;
}

void EngineListWidget::update()
{
    ui->engineList->clear();
    ui->engineList->addItems(EngineSettings::engines());
}

void EngineListWidget::onAdd()
{
    QScopedPointer<AddEngineDialog> dialog(new AddEngineDialog(this));

    if (dialog->exec() == QDialog::Accepted) {
        EngineSettings& settings = SettingsFactory::engineSettings(dialog->name());
        settings.set("stringName", dialog->name());
        settings.set("stringPathExec", dialog->path());
        settings.set("stringPathWork", dialog->workdir());
        settings.set("stringArguments", dialog->arguments());
        settings.save();
        update();
    }
}

void EngineListWidget::onRemove()
{
    if (!ui->engineList->currentItem())
        return;

    EngineSettings::remove(ui->engineList->currentItem()->text());
    update();
}

void EngineListWidget::onConfigure()
{
    QListWidgetItem* current = ui->engineList->currentItem();

    if (!current)
        return;

    EngineSettings& settings = SettingsFactory::engineSettings(current->text());

    QScopedPointer<EngineOptionsDialog> dialog(new EngineOptionsDialog(settings, this));
    dialog->exec();
}

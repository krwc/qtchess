#include "add-engine-dialog.hpp"
#include "ui_add-engine-dialog.h"
#include "settings/settings-factory.hpp"
#include <QDebug>

AddEngineDialog::AddEngineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEngineDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add engine");

    QObject::connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(ui->add, &QPushButton::clicked, this, &AddEngineDialog::onAddClicked);
}

AddEngineDialog::~AddEngineDialog()
{
    delete ui;
}

void AddEngineDialog::onAddClicked()
{
    if (!EngineSettings::engines().contains(ui->name->text())) {
        m_name = ui->name->text();
        m_path = ui->execPath->text();
        m_workdir = ui->workdir->text();
        m_arguments = ui->params->text();
        emit accept();
    } else {
        qWarning() << "TODO: show a dialog instead of doing nothing.";
    }
}

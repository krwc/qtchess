#include "gui/settings/engine-edit-options-dialog.hpp"
#include "ui_engine-edit-options-dialog.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QProgressBar>
#include <QDebug>

EngineEditOptionsDialog::EngineEditOptionsDialog(EngineConfig config, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EngineEditOptionsDialog)
    , m_config(config)
{
    ui->setupUi(this);
    QObject::connect(ui->ok, SIGNAL(clicked(bool)), this, SLOT(onOkClicked()));
    rebuildTable();
}

EngineEditOptionsDialog::~EngineEditOptionsDialog()
{
    delete ui;
}

void EngineEditOptionsDialog::onOkClicked()
{
    emit accept();
}

void EngineEditOptionsDialog::rebuildTable()
{
    QTableWidget* table = ui->table;
    int rowId = 0;

    table->clearContents();
    Engine engine(m_config);
    engine.start();

    for (const EngineOption& option : engine.options()) {
        QWidget* widget;

        switch (option.type()) {
        case EngineOption::String:
            widget = createLineEdit(option.name(), option);
            break;
        case EngineOption::CheckBox:
            widget = createCheckBox(option.name(), option);
            break;
        case EngineOption::SpinBox:
            widget = createSpinBox(option.name(), option);
            break;
        default:
            qDebug() << "FIXME: option " << option.name() << " does not have visual representation.";
            widget = new QWidget();
        }

        table->insertRow(rowId);
        table->setItem(rowId, 0, new QTableWidgetItem(option.name()));
        table->setItem(rowId, 1, new QTableWidgetItem());
        table->setCellWidget(rowId, 1, widget);
        ++rowId;
    }

    // Empty table.
    if (!table->item(0, 0))
        return;

    // Resize table rows to just look good.
    const int height = 1.4 * QFontMetrics(table->item(0, 0)->font()).height();

    for (int i = 0; i < rowId; i++)
        table->setRowHeight(i, height);

    // And columns too
    table->resizeColumnToContents(0);
}

QLineEdit* EngineEditOptionsDialog::createLineEdit(QString key, EngineOption option)
{
    QString currentValue = option.value(EngineOption::Default).toString();

    if (m_config.options().contains(key))
        currentValue = m_config.option(key);

    QLineEdit* widget = new QLineEdit(currentValue);
    QObject::connect(widget, &QLineEdit::textChanged, [this,key,widget] {
        m_config.setOption(key, widget->text());
    });
    return widget;
}

QCheckBox* EngineEditOptionsDialog::createCheckBox(QString key, EngineOption option)
{
    bool currentValue = option.value(EngineOption::Default).toBool();

    if (m_config.options().contains(key))
        currentValue = m_config.option(key).toInt();

    QCheckBox* widget = new QCheckBox();
    widget->setChecked(currentValue);
    QObject::connect(widget, &QCheckBox::clicked, [this,key,widget](bool checked) {
        m_config.setOption(key, checked ? "true" : "false");
    });
    return widget;
}

QSpinBox* EngineEditOptionsDialog::createSpinBox(QString key, EngineOption option)
{
    int currentValue = option.value(EngineOption::Default).toInt();

    if (m_config.options().contains(key))
        currentValue = m_config.option(key).toInt();

    QSpinBox* widget = new QSpinBox();
    widget->setMinimum(option.value(EngineOption::MinValue).toInt());
    widget->setMaximum(option.value(EngineOption::MaxValue).toInt());
    widget->setValue(currentValue);
    QObject::connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this,key,widget](int value) {
        m_config.setOption(key, QString::number(value));
    });
    return widget;
}

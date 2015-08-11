#include "engine-options-dialog.hpp"
#include "ui_engine-options-dialog.h"
#include "settings/settings-factory.hpp"
#include "engine/engine.hpp"
#include <QDebug>

EngineOptionsDialog::EngineOptionsDialog(EngineSettings& settings, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EngineOptionsDialog)
    , m_settings(settings)
    , m_engine(settings)
{
    ui->setupUi(this);
    setWindowTitle("Engine options");

    QObject::connect(&m_engine, &Engine::optionsParsed, this, &EngineOptionsDialog::onOptionsParsed);
    m_engine.start();

    QObject::connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    QObject::connect(ui->cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

EngineOptionsDialog::~EngineOptionsDialog()
{
    delete ui;
}

void EngineOptionsDialog::onSave()
{
    emit accept();
}

void EngineOptionsDialog::onOptionsParsed(QList<EngineOption> options)
{
    for (EngineOption& option : options) {
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
        ui->form->addRow(option.name(), widget);
    }
}
#define BIND_SETTER_FUNCTOR(key,value,widget) \
    QObject::connect(ui->save, &QPushButton::clicked, [this,key,widget] { m_settings.set(key, value); m_settings.save(); })

QLineEdit* EngineOptionsDialog::createLineEdit(QString key, EngineOption option)
{
    QString currentValue = option.value(EngineOption::Default).toString();

    if (m_settings.keys().contains(key))
        currentValue = m_settings.get(key).toString();

    QLineEdit* widget = new QLineEdit(currentValue);
    BIND_SETTER_FUNCTOR(key, widget->text(), widget);
    return widget;
}

QCheckBox* EngineOptionsDialog::createCheckBox(QString key, EngineOption option)
{
    bool currentValue = option.value(EngineOption::Default).toInt();

    if (m_settings.keys().contains(key))
        currentValue = m_settings.get(key).toBool();

    QCheckBox* widget = new QCheckBox();
    widget->setChecked(currentValue);
    BIND_SETTER_FUNCTOR(key, widget->isChecked(), widget);
    return widget;
}

QSpinBox* EngineOptionsDialog::createSpinBox(QString key, EngineOption option)
{
    int currentValue = option.value(EngineOption::Default).toInt();

    if (m_settings.keys().contains(key))
        currentValue = m_settings.get(key).toInt();
    QSpinBox* widget = new QSpinBox();
    widget->setMinimum(option.value(EngineOption::MinValue).toInt());
    widget->setMaximum(option.value(EngineOption::MaxValue).toInt());
    widget->setValue(currentValue);
    BIND_SETTER_FUNCTOR(key, widget->value(), widget);
    return widget;
}

#include "gui/engine-list-widget.hpp"
#include "settings/settings-factory.hpp"
#include "ui_engine-list-widget.h"
#include <QDebug>

EngineListWidget::EngineListWidget(QWidget* parent)
    : QListWidget(parent)
    , m_currentId(-1)
{
    QObject::connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));
    QObject::connect(&SettingsFactory::engines(), &AbstractSettings::changed, this, &EngineListWidget::update);
    update();
}

QString EngineListWidget::engineName() const
{
    return m_engines[m_currentId];
}

bool EngineListWidget::selected() const
{
    return m_currentId != -1;
}

void EngineListWidget::update()
{
    m_engines = SettingsFactory::engines().names();
    m_currentId = -1;
    clear();
    addItems(m_engines);
}

void EngineListWidget::onClicked(QModelIndex index)
{
    m_currentId = index.row();
}



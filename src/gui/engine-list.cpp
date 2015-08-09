#include "gui/engine-list.hpp"
#include "settings.hpp"
#include <QScopedPointer>

EngineDialog::EngineDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Add engine");
}

const QString& EngineDialog::name() const
{
    return m_name;
}

EngineList::EngineList(QWidget* parent)
    : QListWidget(parent)
{
    m_currentList = Settings::instance().get(Settings::EngineList).toStringList();
    update();

}

void EngineList::update()
{
    clear();
    addItems(m_currentList);
}

void EngineList::addEngine()
{
    QScopedPointer<EngineDialog> dialog(new EngineDialog(this));

    if (dialog->exec() == QDialog::Rejected)
        return;
}

void EngineList::delEngine()
{
    if (currentItem()) {
        m_currentList.removeOne(currentItem()->text());
        update();
    }
}

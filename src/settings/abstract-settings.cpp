#include "abstract-settings.hpp"

AbstractSettings::AbstractSettings(const QString& logicalGroup,
                                   const QString& subGroup)
    : m_settings("QtChess", logicalGroup)
{
    if (subGroup != "")
        m_settings.beginGroup(subGroup);
}

QVariant AbstractSettings::get(const QString& key) const
{
    if (m_values.contains(key))
        return m_values[key];
    else if (m_settings.contains(key))
        return m_settings.value(key);

    Q_ASSERT(!"Invalid setting key.");
}

QStringList AbstractSettings::keys() const
{
    return m_settings.childKeys();
}

QStringList AbstractSettings::groups() const
{
    return m_settings.childGroups();
}

void AbstractSettings::set(const QString& key, const QVariant& value)
{
    m_values[key] = value;
    emit changed();
}

void AbstractSettings::unset(const QString& key)
{
    m_settings.remove(key);
}

void AbstractSettings::reset()
{
    for (const QString& key : m_values.keys()) {
        if (m_settings.contains(key))
            set(key, m_settings.value(key));
    }
}

void AbstractSettings::save()
{
    for (const QString& key : m_values.keys())
        m_settings.setValue(key, m_values[key]);
    m_settings.sync();
}

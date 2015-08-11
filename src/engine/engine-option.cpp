#include "engine/engine-option.hpp"

EngineOption::EngineOption(Type type, QString name, QMap<Entry, QVariant> entries)
    : m_entries(entries)
    , m_name(name)
    , m_type(type)
{
}

EngineOption EngineOption::string(QString name, QString defaultValue)
{
    return EngineOption(Type::String, name, {{ Default, defaultValue }});
}

EngineOption EngineOption::button(QString name)
{
    return EngineOption(Type::Button, name, { });
}

EngineOption EngineOption::spinbox(QString name, int minValue, int maxValue,
                                   int defaultValue)
{
    QMap<Entry, QVariant> map;
    map[Default] = defaultValue;
    map[MinValue] = minValue;
    map[MaxValue] = maxValue;
    return EngineOption(Type::SpinBox, name, map);
}

EngineOption EngineOption::checkbox(QString name, bool value)
{
    return EngineOption(Type::CheckBox, name, {{ Default, value }});
}

EngineOption EngineOption::combobox(QString name, QString defaultValue, QStringList values)
{
    return EngineOption(Type::ComboBox, name, {{ Default, defaultValue }, { Vars, values }});
}

QVariant EngineOption::value(Entry entry) const
{
    Q_ASSERT(m_entries.contains(entry));
    return m_entries[entry];
}

EngineOption::Type EngineOption::type() const
{
    return m_type;
}

QString EngineOption::name() const
{
    return m_name;
}

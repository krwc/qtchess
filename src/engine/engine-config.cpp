#include "engine/engine-config.hpp"

EngineConfig::EngineConfig(const QVariantMap& map)
{
    m_name    = map["name"].toString();
    m_command = map["command"].toString();
    m_workdir = map["workdir"].toString();
    m_arguments = map["arguments"].toString();
    m_options = map["options"].toMap();
}


void EngineConfig::setName(const QString& name)
{
    m_name = name;
}

void EngineConfig::setCommand(const QString& command)
{
    m_command = command;
}

void EngineConfig::setWorkdir(const QString& workdir)
{
    m_workdir = workdir;
}

void EngineConfig::setArguments(const QString& arguments)
{
    m_arguments = arguments;
}

QString EngineConfig::name() const
{
    return m_name;
}

QString EngineConfig::command() const
{
    return m_command;
}

QString EngineConfig::workdir() const
{
    return m_workdir;
}

QString EngineConfig::arguments() const
{
    return m_arguments;
}

void EngineConfig::setOption(const QString& name, const QString& value)
{
    m_options[name] = value;
}

QString EngineConfig::option(const QString& name) const
{
    Q_ASSERT(m_options.contains(name));
    return m_options[name].toString();
}

QStringList EngineConfig::options() const
{
    return m_options.keys();
}

QVariantMap EngineConfig::toVariantMap() const
{
    QVariantMap map;
    map["name"] = m_name;
    map["command"] = m_command;
    map["workdir"] = m_workdir;
    map["arguments"] = m_arguments;
    map["options"] = m_options;
    return map;
}

bool EngineConfig::operator== (const EngineConfig& rhs) const
{
    if (this == &rhs)
        return true;

    return m_name == rhs.m_name &&
           m_command == rhs.m_command &&
           m_workdir == rhs.m_workdir &&
           m_arguments == rhs.m_arguments &&
           m_options == rhs.m_options;
}

bool EngineConfig::operator!= (const EngineConfig& rhs) const
{
    return !(*this == rhs);
}

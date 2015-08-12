#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP
#include <QVariantMap>
#include "engine/engine-option.hpp"

class EngineConfig {
public:
    EngineConfig() = default;

    /*! \brief Initializes instance from variant map */
    EngineConfig(const QVariantMap& map);

    void setName(const QString& name);
    void setCommand(const QString& command);
    void setWorkdir(const QString& workdir);
    void setArguments(const QString& arguments);

    QString name() const;
    QString command() const;
    QString workdir() const;
    QString arguments() const;

    /*! \brief Sets option value */
    void setOption(const QString& name, const QString& value);

    /*! \brief Gets stringified value of the option */
    QString option(const QString& name) const;

    /*! \brief Gets list of options */
    QStringList options() const;

    /*! \brief Serializes this instance to variant map */
    QVariantMap toVariantMap() const;

    /*! \brief Compares whether instances of EngineConfig are equal */
    bool operator== (const EngineConfig& rhs) const;
    bool operator!= (const EngineConfig& rhs) const;
private:
    QString m_name;
    QString m_command;
    QString m_workdir;
    QString m_arguments;
    QVariantMap m_options;
};

#endif // ENGINE_SETTINGS_HPP

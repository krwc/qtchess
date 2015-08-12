#ifndef ENGINES_SETTINGS_HPP
#define ENGINES_SETTINGS_HPP
#include "settings/abstract-settings.hpp"

class EngineConfig;

class EnginesSettings : public AbstractSettings {
    friend class SettingsFactory;
private:
    EnginesSettings();
public:
    /*! \brief Returns list of registered engines names */
    QStringList names() const;

    /*! \brief Saves engine config. */
    void saveConfig(const EngineConfig& config);

    /*! \breif Removes engine. */
    void remove(const QString& name);

    /*! \brief Returns config of an engine of the given name. */
    EngineConfig config(const QString& name) const;
};

#endif

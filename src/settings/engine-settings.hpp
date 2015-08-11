#ifndef ENGINE_SETTINGS_HPP
#define ENGINE_SETTINGS_HPP
#include "settings/abstract-settings.hpp"

class EngineSettings : public AbstractSettings {
    friend class SettingsFactory;
private:
    EngineSettings(const QString& engineName);
public:
    static QStringList engines();
    static void remove(const QString& engine);
};

#endif

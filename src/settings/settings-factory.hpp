#ifndef SETTINGS_FACTORY_HPP
#define SETTINGS_FACTORY_HPP
#include "settings/engine-settings.hpp"
#include "settings/board-settings.hpp"
#include "settings/html-settings.hpp"
#include <memory>

class SettingsFactory
{
public:
    SettingsFactory() = delete;

    static BoardSettings& boardSettings()
    {
        static BoardSettings settings;
        return settings;
    }

    static HtmlSettings& htmlSettings()
    {
        static HtmlSettings settings;
        return settings;
    }

    static EngineSettings& engineSettings(const QString& engineName)
    {
        static QMap<QString, std::shared_ptr<EngineSettings>> settings;

        if (!settings.contains(engineName))
            // Cannot use make_shared because of constructor privateness.
            settings[engineName] = std::shared_ptr<EngineSettings>(new EngineSettings(engineName));

        return *settings[engineName];
    }
};

#endif // SETTINGS_FACTORY_HPP

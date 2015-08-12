#ifndef SETTINGS_FACTORY_HPP
#define SETTINGS_FACTORY_HPP
#include "settings/engines-settings.hpp"
#include "settings/board-settings.hpp"
#include "settings/html-settings.hpp"
#include <memory>

class SettingsFactory
{
public:
    SettingsFactory() = delete;

    static BoardSettings& board()
    {
        static BoardSettings settings;
        return settings;
    }

    static HtmlSettings& html()
    {
        static HtmlSettings settings;
        return settings;
    }

    static EnginesSettings& engines()
    {
        static EnginesSettings settings;
        return settings;
        /*
        static QMap<QString, std::shared_ptr<EngineSettings>> settings;

        if (!settings.contains(engineName))
            // Cannot use make_shared because of constructor privateness.
            settings[engineName] = std::shared_ptr<EngineSettings>(new EngineSettings(engineName));

        return *settings[engineName];*/
    }
};

#endif // SETTINGS_FACTORY_HPP

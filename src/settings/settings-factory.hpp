#ifndef SETTINGS_FACTORY_HPP
#define SETTINGS_FACTORY_HPP
#include "settings/board-settings.hpp"
#include "settings/engines-settings.hpp"
#include "settings/html-settings.hpp"
#include "settings/layout-settings.hpp"
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
    }

    static LayoutSettings &layout() {
        static LayoutSettings settings;
        return settings;
    }
};

#endif // SETTINGS_FACTORY_HPP

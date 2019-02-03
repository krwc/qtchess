#ifndef LAYOUT_SETTINGS_HPP
#define LAYOUT_SETTINGS_HPP
#include "settings/abstract-settings.hpp"

class LayoutSettings : public AbstractSettings {
    friend class SettingsFactory;
private:
    LayoutSettings();
public:
    static constexpr const char *MAIN_WINDOW_GEOMETRY = "mainWindowGeometry";
    static constexpr const char *MAIN_WINDOW_STATE = "mainWindowState";
};

#endif

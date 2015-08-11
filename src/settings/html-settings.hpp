#ifndef HTML_SETTINGS_HPP
#define HTML_SETTINGS_HPP
#include "settings/abstract-settings.hpp"

class HtmlSettings : public AbstractSettings {
    friend class SettingsFactory;
private:
    HtmlSettings();
};

#endif

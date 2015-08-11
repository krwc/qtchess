#ifndef BOARD_SETTINGS_HPP
#define BOARD_SETTINGS_HPP
#include "settings/abstract-settings.hpp"

class BoardSettings : public AbstractSettings {
    friend class SettingsFactory;
private:
    BoardSettings();
};

#endif

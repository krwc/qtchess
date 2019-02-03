#include "settings/layout-settings.hpp"

LayoutSettings::LayoutSettings()
    : AbstractSettings("layout")
{
    set(MAIN_WINDOW_GEOMETRY, QByteArray{});
    set(MAIN_WINDOW_STATE, QByteArray{});
    reset();
}

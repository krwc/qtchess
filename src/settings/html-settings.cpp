#include "settings/html-settings.hpp"
#include <QColor>

HtmlSettings::HtmlSettings()
    : AbstractSettings("html")
{
    set("colorMove"          , QColor(Qt::black));
    set("colorVariant"       , QColor(Qt::darkGreen));
    set("colorBackground"    , QColor(Qt::white));
    set("colorMoveNumber"    , QColor(Qt::black));
    set("colorMoveHighlight" , QColor(Qt::white));
    set("colorHighlight"     , QColor(Qt::black));
    set("colorAnnotation"    , QColor(Qt::blue));
    reset();
}

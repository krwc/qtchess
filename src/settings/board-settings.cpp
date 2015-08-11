#include "gui/board/board-widget.hpp"
#include "board-settings.hpp"
#include <QColor>

BoardSettings::BoardSettings()
    : AbstractSettings("board")
{
    set("stringPieceStyle"  , "ksi");
    set("colorSquareLight"  , QColor("#fffefb"));
    set("colorSquareDark"   , QColor("#84a4d3"));
    set("colorPicking"      , QColor("#0a1ca0"));
    set("colorBorder"       , QColor("#282828"));
    set("colorBorderText"   , QColor(Qt::white));
    set("boolBorder"        , true);
    set("intMarginSize"     , 5);
    set("intBorderSize"     , 20);
    reset();
}


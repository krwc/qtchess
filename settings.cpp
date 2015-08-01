#include "settings.hpp"
#include <QDebug>
#include <QDir>

static const QString KEY_PIECE_STYLE_NAME = "piece_style_name";
static const QString KEY_LIGHT_SQUARE_COLOR = "light_square_color";
static const QString KEY_DARK_SQUARE_COLOR = "dark_square_color";
static const QString KEY_SELECTION_COLOR = "selection_color";
static const QString KEY_SHOULD_DRAW_COORDS = "should_draw_coords";
static const QString KEY_BORDER_SIZE = "border_size";
static const QString KEY_MARGIN_SIZE = "margin_size";
static const QString KEY_THEME_NAME = "theme_name";

static const QString ThemesPath = "assets/themes";

static const Settings::SettingsStruct Default = {
    .PieceStyleName = "default",
    .LightSquareColor = QColor(255, 255, 255),
    .DarkSquareColor = QColor(115, 158, 206),
    .SelectionColor = QColor(10, 28, 160),
    .ShouldDrawCoords = true,
    .BorderSize = 20,
    .MarginSize = 5,
    .ThemeName = "dark.css"
};

Settings::Settings()
  : mConfig(Default)
  , mSettings("QtChess", "QtChess")
  , mPieceSet(nullptr)
  , mTheme(nullptr)
{
    load();
}

void Settings::reset()
{
    // Simply reload from file.
    load();
}

void Settings::save()
{
    mSettings.setValue(KEY_PIECE_STYLE_NAME, mConfig.PieceStyleName);
    mSettings.setValue(KEY_LIGHT_SQUARE_COLOR, mConfig.LightSquareColor);
    mSettings.setValue(KEY_DARK_SQUARE_COLOR, mConfig.DarkSquareColor);
    mSettings.setValue(KEY_SELECTION_COLOR, mConfig.SelectionColor);
    mSettings.setValue(KEY_SHOULD_DRAW_COORDS, mConfig.ShouldDrawCoords);
    mSettings.setValue(KEY_BORDER_SIZE, mConfig.BorderSize);
    mSettings.setValue(KEY_MARGIN_SIZE, mConfig.MarginSize);
    mSettings.setValue(KEY_THEME_NAME, mConfig.ThemeName);
}

void Settings::load()
{
    mConfig.BorderSize = mSettings.value(KEY_BORDER_SIZE, Default.BorderSize).toInt();
    mConfig.MarginSize = mSettings.value(KEY_MARGIN_SIZE, Default.MarginSize).toInt();
    mConfig.ShouldDrawCoords = mSettings.value(KEY_SHOULD_DRAW_COORDS, Default.ShouldDrawCoords).toBool();
    mConfig.SelectionColor = mSettings.value(KEY_SELECTION_COLOR, Default.SelectionColor).value<QColor>();
    mConfig.LightSquareColor = mSettings.value(KEY_LIGHT_SQUARE_COLOR, Default.LightSquareColor).value<QColor>();
    mConfig.DarkSquareColor = mSettings.value(KEY_DARK_SQUARE_COLOR, Default.DarkSquareColor).value<QColor> ();

    setThemeName(mSettings.value(KEY_THEME_NAME, Default.ThemeName).toString());
    setPieceStyleName(mSettings.value(KEY_PIECE_STYLE_NAME, Default.PieceStyleName).toString());
}

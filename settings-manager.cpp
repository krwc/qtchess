#include "settings-manager.hpp"

static const QString KEY_PIECE_STYLE_NAME = "piece_style_name";
static const QString KEY_LIGHT_SQUARE_COLOR_R = "light_square_color_r";
static const QString KEY_LIGHT_SQUARE_COLOR_G = "light_square_color_g";
static const QString KEY_LIGHT_SQUARE_COLOR_B = "light_square_color_b";
static const QString KEY_DARK_SQUARE_COLOR_R = "dark_square_color_r";
static const QString KEY_DARK_SQUARE_COLOR_G = "dark_square_color_g";
static const QString KEY_DARK_SQUARE_COLOR_B = "dark_square_color_b";
static const QString KEY_SELECTION_COLOR_R = "selection_color_r";
static const QString KEY_SELECTION_COLOR_G = "selection_color_g";
static const QString KEY_SELECTION_COLOR_B = "selection_color_b";
static const QString KEY_SHOULD_DRAW_COORDS = "should_draw_coords";
static const QString KEY_BORDER_SIZE = "border_size";
static const QString KEY_MARGIN_SIZE = "margin_size";

static const Settings Default = {
    .PieceStyleName = "default",
    .LightSquareColor = QColor(255, 255, 255),
    .DarkSquareColor = QColor(115, 158, 206),
    .SelectionColor = QColor(10, 28, 160),
    .ShouldDrawCoords = true,
    .BorderSize = 20,
    .MarginSize = 5
};

SettingsManager::SettingsManager()
  : mConfig(Default)
  , mSettings("QtChess", "QtChess")
  , mPieceSet(new PieceSet(Default.PieceStyleName))
{
    load();
}

void SettingsManager::reset()
{
    // Simply reload from file.
    load();
    delete mPieceSet;
    // Replace piece set if needed
    mPieceSet = new PieceSet(mConfig.PieceStyleName);
}

void SettingsManager::save()
{
    mSettings.setValue(KEY_PIECE_STYLE_NAME, mConfig.PieceStyleName);
    mSettings.setValue(KEY_LIGHT_SQUARE_COLOR_R, mConfig.LightSquareColor.red());
    mSettings.setValue(KEY_LIGHT_SQUARE_COLOR_G, mConfig.LightSquareColor.green());
    mSettings.setValue(KEY_LIGHT_SQUARE_COLOR_B, mConfig.LightSquareColor.blue());
    mSettings.setValue(KEY_DARK_SQUARE_COLOR_R, mConfig.DarkSquareColor.red());
    mSettings.setValue(KEY_DARK_SQUARE_COLOR_G, mConfig.DarkSquareColor.green());
    mSettings.setValue(KEY_DARK_SQUARE_COLOR_B, mConfig.DarkSquareColor.blue());
    mSettings.setValue(KEY_SELECTION_COLOR_R, mConfig.SelectionColor.red());
    mSettings.setValue(KEY_SELECTION_COLOR_G, mConfig.SelectionColor.green());
    mSettings.setValue(KEY_SELECTION_COLOR_B, mConfig.SelectionColor.blue());
    mSettings.setValue(KEY_SHOULD_DRAW_COORDS, mConfig.ShouldDrawCoords);
    mSettings.setValue(KEY_BORDER_SIZE, mConfig.BorderSize);
    mSettings.setValue(KEY_MARGIN_SIZE, mConfig.MarginSize);
}

void SettingsManager::load()
{
    mConfig.BorderSize = mSettings.value(KEY_BORDER_SIZE,
                                         Default.BorderSize).toInt();
    mConfig.MarginSize = mSettings.value(KEY_MARGIN_SIZE,
                                         Default.MarginSize).toInt();
    mConfig.ShouldDrawCoords = mSettings.value(KEY_SHOULD_DRAW_COORDS,
                                               Default.ShouldDrawCoords).toBool();
    mConfig.SelectionColor.setRed(mSettings.value(KEY_SELECTION_COLOR_R,
                                                  Default.SelectionColor.red()).toInt());
    mConfig.SelectionColor.setGreen(mSettings.value(KEY_SELECTION_COLOR_G,
                                                    Default.SelectionColor.green()).toInt());
    mConfig.SelectionColor.setBlue(mSettings.value(KEY_SELECTION_COLOR_B,
                                                   Default.SelectionColor.blue()).toInt());

    mConfig.LightSquareColor.setRed(mSettings.value(KEY_LIGHT_SQUARE_COLOR_R,
                                                  Default.LightSquareColor.red()).toInt());
    mConfig.LightSquareColor.setGreen(mSettings.value(KEY_LIGHT_SQUARE_COLOR_G,
                                                    Default.LightSquareColor.green()).toInt());
    mConfig.LightSquareColor.setBlue(mSettings.value(KEY_LIGHT_SQUARE_COLOR_B,
                                                   Default.LightSquareColor.blue()).toInt());

    mConfig.DarkSquareColor.setRed(mSettings.value(KEY_DARK_SQUARE_COLOR_R,
                                                   Default.DarkSquareColor.red()).toInt());
    mConfig.DarkSquareColor.setGreen(mSettings.value(KEY_DARK_SQUARE_COLOR_G,
                                                     Default.DarkSquareColor.green()).toInt());
    mConfig.DarkSquareColor.setBlue(mSettings.value(KEY_DARK_SQUARE_COLOR_B,
                                                    Default.DarkSquareColor.blue()).toInt());

    mConfig.PieceStyleName = mSettings.value(KEY_PIECE_STYLE_NAME, Default.PieceStyleName).toString();
}

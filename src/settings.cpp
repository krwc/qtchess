#include "settings.hpp"
#include <QDebug>
#include <QDir>

Settings::Settings()
  : mSettings("QtChess", "QtChess")
{
    mValues[Settings::PieceStyleName]   = Entry("piece_style_name", "default");
    mValues[Settings::LightSquareColor] = Entry("light_square_color", QColor(255, 255, 255));
    mValues[Settings::DarkSquareColor]  = Entry("dark_square_color", QColor(0, 0, 0));
    mValues[Settings::SelectionColor]   = Entry("selection_color", QColor(10, 28, 160));
    mValues[Settings::ShouldDrawCoords] = Entry("should_draw_coords", true);
    mValues[Settings::BorderSize]       = Entry("border_size", 20);
    mValues[Settings::MarginSize]       = Entry("margin_size", 5);
    mValues[Settings::EnginePath]       = Entry("engine_path", "");

    load();
}

Settings& Settings::instance() {
    static Settings settings;
    return settings;
}

QVariant Settings::get(Settings::Key key) {
    return mValues[key].currentValue();
}

void Settings::set(Settings::Key key, const QVariant& value) {
    mValues[key].setValue(value);
    emit changed();
}

void Settings::reset() {
    // Simply reload from file.
    load();
}

void Settings::save() {
    for (const Entry& entry : mValues.values())
        mSettings.setValue(entry.name(), entry.currentValue());
}

void Settings::load() {
    for (Key key : mValues.keys()) {
        Entry& entry = mValues[key];
        entry.setValue(mSettings.value(entry.name(), entry.defaultValue()));
    }

    emit changed();
}

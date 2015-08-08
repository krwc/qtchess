#include "settings.hpp"
#include <QDebug>
#include <QDir>

Settings::Settings()
  : m_settings("QtChess", "QtChess")
{
    m_values[Settings::PieceStyleName]     = Entry("board/piece_style", "default");
    m_values[Settings::LightSquareColor]   = Entry("board/light_square_color", QColor("#fffefb"));
    m_values[Settings::DarkSquareColor]    = Entry("board/dark_square_color", QColor("#84a4d3"));
    m_values[Settings::SelectionColor]     = Entry("board/selection_color", QColor(10, 28, 160));
    m_values[Settings::PgnBackgroundColor] = Entry("pgn/bg_color", QColor(Qt::white));
    m_values[Settings::PgnMoveColor]       = Entry("pgn/move_color", QColor(Qt::black));
    m_values[Settings::PgnHiMoveColor]     = Entry("pgn/hi_move_color", QColor(Qt::white));
    m_values[Settings::PgnHiColor]         = Entry("pgn/hi_color", QColor(Qt::black));
    m_values[Settings::PgnNumberColor]     = Entry("pgn/number_color", QColor(Qt::black));
    m_values[Settings::PgnVariationColor]  = Entry("pgn/number_color", QColor(Qt::black));
    m_values[Settings::CoordsBorderColor]  = Entry("coords/border_color", QColor("#282828"));
    m_values[Settings::CoordsTextColor]    = Entry("coords/text_color", QColor(Qt::white));
    m_values[Settings::ShouldDrawCoords]   = Entry("board/should_draw_coords", true);
    m_values[Settings::BorderSize]         = Entry("coords/border_size", 20);
    m_values[Settings::MarginSize]         = Entry("coords/margin_size", 5);
    m_values[Settings::EngineList]         = Entry("engine_list", QStringList());
    m_values[Settings::CurrentEngine]      = Entry("engine_current", "");

    load();
}

Settings& Settings::instance() {
    static Settings settings;
    return settings;
}

QVariant Settings::get(Settings::Key key) {
    return m_values[key].currentValue();
}

void Settings::set(Settings::Key key, const QVariant& value) {
    m_values[key].setValue(value);
    emit changed();
}

QVariant Settings::get(const QString& key)
{
    Q_ASSERT(m_settings.contains(key));
    // Now we can pretty much safely return.
    return m_settings.value(key);
}

void Settings::set(const QString& key, const QVariant& value)
{
    m_settings.setValue(key, value);
    emit changed();
}

void Settings::reset() {
    // Simply reload from file.
    load();
}

void Settings::save() {
    for (const Entry& entry : m_values.values())
        m_settings.setValue(entry.name(), entry.currentValue());
}

void Settings::load() {
    for (Key key : m_values.keys()) {
        Entry& entry = m_values[key];
        entry.setValue(m_settings.value(entry.name(), entry.defaultValue()));
    }

    emit changed();
}

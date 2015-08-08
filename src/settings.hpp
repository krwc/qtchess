#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QSettings>
#include <QColor>
#include <QDebug>
#include <QList>
#include <QMap>


class Settings : public QObject {
    Q_OBJECT
public:
    enum Key {
        PieceStyleName, LightSquareColor, DarkSquareColor,
        SelectionColor, PgnBackgroundColor, PgnMoveColor, PgnHiMoveColor,
        PgnHiColor, PgnNumberColor, PgnVariationColor, CoordsBorderColor,
        CoordsTextColor, ShouldDrawCoords, BorderSize, MarginSize,
        ThemeName, EngineList, CurrentEngine
    };

    class Entry {
    private:
        QString m_name;
        QVariant m_currentValue;
        QVariant m_defaultValue;
    public:
        Entry()
          : m_name("")
          , m_currentValue(0)
          , m_defaultValue(0)
        { }

        Entry(const QString& name, const QVariant& defaultValue) 
          : m_name(name)
          , m_currentValue(defaultValue)
          , m_defaultValue(defaultValue)
        { }

        QVariant currentValue() const {
            return m_currentValue;
        }

        QVariant defaultValue() const {
            return m_defaultValue;
        }

        QString name() const {
            return m_name;
        }

        void setValue(QVariant value) {
            m_currentValue = value;
        }
    };
private:
    /** Loaded settings values from file */
    QMap<Key, Entry> m_values;
    Settings();
public:
    /** Returns settings singleton instance */
    static Settings& instance();

    /** Returns value of an element of given key */
    QVariant get(Settings::Key key);
    /** Sets value of an element of given key */
    void set(Settings::Key key, const QVariant& value);

    /*! \brief Returns value of an element of the given string-key. */
    QVariant get(const QString& key);
    /*! \brief Sets value of an element of given key */
    void set(const QString& key, const QVariant& value);

    void reset();
    void save();
signals:
    /*! \brief signal emited whenever some setting is overwritten */
    void changed();
private:
    void load();
    QSettings m_settings;
};

#endif // SETTINGS_HPP

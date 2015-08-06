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
        SelectionColor, ShouldDrawCoords, BorderSize, MarginSize,
        EnginePath, ThemeName
    };

    class Entry {
    private:
        QString mName;
        QVariant mCurrentValue;
        QVariant mDefaultValue;
    public:
        Entry()
          : mName("")
          , mCurrentValue(0)
          , mDefaultValue(0)
        { }

        Entry(const QString& name, const QVariant& defaultValue) 
          : mName(name)
          , mCurrentValue(defaultValue)
          , mDefaultValue(defaultValue)
        { }

        QVariant currentValue() const {
            return mCurrentValue;
        }

        QVariant defaultValue() const {
            return mDefaultValue;
        }

        QString name() const {
            return mName;
        }

        void setValue(QVariant value) {
            mCurrentValue = value;
        }
    };
private:
    /** Loaded settings values from file */
    QMap<Key, Entry> mValues;
    Settings();
public:
    /** Returns settings singleton instance */
    static Settings& instance();

    /** Returns value of an element of given key */
    QVariant get(Settings::Key key);
    /** Sets value of an element of given key */
    void set(Settings::Key key, const QVariant& value);
    void reset();
    void save();
signals:
    /*! \brief signal emited whenever some setting is overwritten */
    void changed();
private:
    void load();
    QSettings mSettings;
};

#endif // SETTINGS_HPP

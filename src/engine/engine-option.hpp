#ifndef ENGINE_OPTION_HPP
#define ENGINE_OPTION_HPP
#include <QVariantMap>

class EngineOption {
public:
    EngineOption() = default;

    enum Type {
        String,
        Button,
        SpinBox,
        CheckBox,
        ComboBox,
    };

    enum Entry {
        MinValue,
        MaxValue,
        Default,
        Vars
    };

    /*! \brief Constructs string option. */
    static EngineOption string(QString name, QString defaultValue);

    /*! \brief Constructs button option. */
    static EngineOption button(QString name);

    /*! \brief Constructs spinbox option. */
    static EngineOption spinbox(QString name, int minValue, int maxValue, int defaultValue);

    /*! \brief Constructs checkbox option. */
    static EngineOption checkbox(QString name, bool value);

    /*! \brief Constructs combobox option. */
    static EngineOption combobox(QString name, QString defaultValue, QStringList values);

    /*! \brief Returns value of an entry or throws if it is not available. */
    QVariant value(Entry entry) const;

    /*! \brief Returns this option type */
    Type type() const;

    /*! \brief Returns name */
    QString name() const;
private:
    EngineOption(Type type, QString name, QMap<Entry, QVariant> entries);

    QMap<Entry, QVariant> m_entries;
    QString m_name;
    Type m_type;
};

#endif // ENGINE_OPTION_HPP


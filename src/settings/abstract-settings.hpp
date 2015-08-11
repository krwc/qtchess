#ifndef ABSTRACT_SETTINGS_HPP
#define ABSTRACT_SETTINGS_HPP
#include <QVariant>
#include <QSettings>

class AbstractSettings : public QObject {
    Q_OBJECT
public:
    AbstractSettings(const QString& logicalGroup, const QString& subGroup = "");

    /*! \brief Gets value when the key exists either in memory or in file, otherwise throws assertion. */
    virtual QVariant get(const QString& key) const;

    /*! \brief Lists all keys in this logical settings group. */
    virtual QStringList keys() const;

    /*! \brief Lists all groups */
    virtual QStringList groups() const;

    /*! \brief Assigns value to the key and saves it in memory. */
    virtual void set(const QString& key, const QVariant& value);

    /*! \brief Removes key and its children. */
    virtual void unset(const QString& key);

    /*! \brief Resets all memory saved keys to their default values from file. */
    virtual void reset();

    /*! \brief Saves all keys to file. */
    virtual void save();
signals:
    /*! \brief Emited whenever setValue() or reset() is called. */
    void changed();
private:
    QSettings m_settings;
    QVariantMap m_values;
};

#endif

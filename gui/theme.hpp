#ifndef THEME_HPP
#define THEME_HPP
#include <QDir>

class Theme {
public:
    Theme(QString Name);

    const QString& getSource() const {
        return mSource;
    }

    static QStringList getAvailableThemes();
private:
    QString mSource;
};

#endif // THEME_HPP

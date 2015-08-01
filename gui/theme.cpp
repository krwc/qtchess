#include "theme.hpp"
#include <stdexcept>
#include <QTextStream>

static const QString ThemesPath = "assets/themes";

void LoadTheme(QString& Result, QString Name) {
    QFile ThemeFile(ThemesPath + "/" + Name);
    if (!ThemeFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Cannot open theme.");

    QTextStream Stream(&ThemeFile);
    while (!Stream.atEnd())
        Result += Stream.readLine();
}

QStringList Theme::getAvailableThemes() {
    return QDir(ThemesPath).entryList(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
}

Theme::Theme(QString Name) {
    LoadTheme(mSource, Name);
}

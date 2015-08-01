#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP
#include <piece-set.hpp>
#include <gui/theme.hpp>
#include <QSettings>
#include <QColor>
#include <QDebug>
#include <QList>

struct Settings {
    QString PieceStyleName;
    QColor LightSquareColor;
    QColor DarkSquareColor;
    QColor SelectionColor;
    bool ShouldDrawCoords;
    int BorderSize;
    int MarginSize;

    QString EnginePath;
    QString ThemeName;
};

class SettingsManager
{
public:
    SettingsManager();

    QString getPieceStyleName() const {
        return mConfig.PieceStyleName;
    }

    QColor getLSColor() const {
        return mConfig.LightSquareColor;
    }

    QColor getDSColor() const {
        return mConfig.DarkSquareColor;
    }

    QColor getSelectionColor() const {
        return mConfig.SelectionColor;
    }

    bool getShouldDrawCoords() const {
        return mConfig.ShouldDrawCoords;
    }

    int getBorderSize() const {
        return mConfig.BorderSize;
    }

    int getMarginSize() const {
        return mConfig.MarginSize;
    }

    QString getEnginePath() const {
        return mConfig.EnginePath;
    }

    QString getThemeName() const {
        return mConfig.ThemeName;
    }

    PieceSet& getPieceSet() {
        return *mPieceSet;
    }

    Theme& getTheme() {
        return *mTheme;
    }

    void setPieceStyleName(QString Name) {
        mConfig.PieceStyleName = Name;
        delete mPieceSet;
        mPieceSet = new PieceSet(Name);
    }

    void setLSColor(QColor Color) {
        mConfig.LightSquareColor = Color;
    }

    void setDSColor(QColor Color) {
        mConfig.DarkSquareColor = Color;
    }

    void setSelectionColor(QColor Color) {
        mConfig.SelectionColor = Color;
    }

    void setShouldDrawCoords(bool Value) {
        mConfig.ShouldDrawCoords = Value;
    }

    void setBorderSize(int Size) {
        mConfig.BorderSize = Size;
    }

    void setMarginSize(int Size) {
        mConfig.MarginSize = Size;
    }

    void setEnginePath(QString Value) {
        mConfig.EnginePath = Value;
    }

    void setThemeName(QString Value) {
        mConfig.PieceStyleName = Value;
        delete mTheme;
        mTheme = new Theme(Value);
    }

    void reset();
    void save();
private:
    void load();
    Settings mConfig;
    QSettings mSettings;
    PieceSet* mPieceSet;
    Theme* mTheme;
};

#endif // SETTINGSMANAGER_HPP

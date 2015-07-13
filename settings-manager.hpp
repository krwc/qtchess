#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP
#include "piece-set.hpp"
#include <QSettings>
#include <QColor>

struct Settings {
    QString PieceStyleName;
    QColor LightSquareColor;
    QColor DarkSquareColor;
    QColor SelectionColor;
    bool ShouldDrawCoords;
    int BorderSize;
    int MarginSize;
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

    PieceSet& getPieceSet() {
        return *mPieceSet;
    }

    void setPieceStyleName(QString Name) {
        mConfig.PieceStyleName = Name;
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

    void reset();
    void save();
private:
    void load();
    Settings mConfig;
    QSettings mSettings;
    PieceSet* mPieceSet;
};

#endif // SETTINGSMANAGER_HPP

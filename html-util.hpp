#ifndef HTMLUTIL_HPP
#define HTMLUTIL_HPP
#include <QString>
#include <QColor>

namespace Html {

enum FontWeight {
    Bold,
    Normal
};

enum FontDecoration {
    None
};

static QString LineBreak = "<br>";

class StyleBuilder {
private:
    static const QString FONT_WEIGHT;
    static const QString FONT_COLOR;
    static const QString TEXT_DECORATION;
    static const QString FONT_FAMILY;
    static const QString FONT_SIZE;
    static const QString LINE_HEIGHT;

    static QString fontWeight(FontWeight Weight) {
        QString Ret = FONT_WEIGHT;
        switch (Weight) {
        case Bold:
            Ret += "bold;"; break;
        default: case Normal:
            Ret += "normal;"; break;
        };
        return Ret;
    }

    static QString fontDecoration(FontDecoration Decoration) {
        switch (Decoration) {
        default: case None:
            return TEXT_DECORATION + "none;";
        }
    }
public:
    StyleBuilder() { }

    StyleBuilder& setFontWeight(FontWeight Weight) {
        mStr += fontWeight(Weight);
        return *this;
    }

    StyleBuilder& setFontDecoration(FontDecoration Decoration) {
        mStr += fontDecoration(Decoration);
        return *this;
    }

    StyleBuilder& setFontFamily(QString Family) {
        mStr += FONT_FAMILY + Family + ";";
        return *this;
    }

    StyleBuilder& setFontSizeInPx(int Pixels) {
        mStr += FONT_SIZE + QString::number(Pixels) + "px;";
        return *this;
    }

    StyleBuilder& setLineHeight(int Percentage) {
        mStr += LINE_HEIGHT + QString::number(Percentage) + "%;";
        return *this;
    }

    StyleBuilder& setFontColor(QColor Color) {
        mStr += FONT_COLOR + Color.name();
        return *this;
    }

    operator QString() const {
        return mStr;
    }
private:
    QString mStr;
};

class TagBuilder {
public:
    TagBuilder(QString Name) : mName(Name) { }

    TagBuilder& setProperty(QString Property, QString Value) {
        mProperties += " " + Property + "='" + Value + "'";
        return *this;
    }

    TagBuilder& appendInnerText(QString Text) {
        mInnerText += Text;
        return *this;
    }

    operator QString() const {
        return "<"+mName+mProperties+">"+mInnerText+"</"+mName+">";
    }
private:
    QString mName;
    QString mProperties;
    QString mInnerText;
};
}


#endif // HTMLUTIL_HPP


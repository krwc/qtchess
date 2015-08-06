#ifndef COLOR_BUTTON_HPP
#define COLOR_BUTTON_HPP
#include <QPushButton>
#include <QColorDialog>
#include <QColor>

class ColorButton : public QPushButton {
    Q_OBJECT
public:
    /*! \brief constructs transparent color button */
    explicit ColorButton(QWidget* parent = 0, const QColor& color = QColor(0, 0, 0, 0));

    /*! \brief returns currently selected color */
    const QColor& color() const { return m_currentColor; }

    /*! \brief sets current color */
    void setColor(const QColor& color) { onChanged(color); }

    /*! \brief overloaded to disallow drawing text on the button */
    virtual void setText(const QString& text);
signals:
    /*! \brief signal emited when color has been selected in color chooser */
    void changed(QColor);
private slots:
    /*! \brief called when color has been changed */
    void onChanged(const QColor&);
    /*! \brief called when somebody clicked the button */
    void onClicked(bool);
private:
    QColor m_currentColor;
};

#endif // color-button.hpp

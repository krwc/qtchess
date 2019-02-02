#include "gui/color-button.hpp"
#include <QPainter>
#include <QDebug>
#include <QPalette>
#include <QScopedPointer>

ColorButton::ColorButton(QWidget* parent, const QColor& color)
    : QPushButton("", parent)
    , m_currentColor(color)
{
    // NOTE: Without button being flat it may not always be filled with color.
    setFlat(true);
    setAutoFillBackground(true);

    emit changed(color);
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
    QObject::connect(this, SIGNAL(changed(QColor)), this, SLOT(onChanged(QColor)));
}

void ColorButton::onChanged(const QColor& color)
{
    m_currentColor = color;
    setPalette(QPalette(color));
}

void ColorButton::onClicked(bool)
{
    QScopedPointer<QColorDialog> dialog(
        new QColorDialog(m_currentColor, parentWidget()));

    if (dialog->exec() != QDialog::Rejected)
        emit changed(dialog->selectedColor());
}

void ColorButton::setText(const QString&)
{
    // Changing text is prohibited.
}

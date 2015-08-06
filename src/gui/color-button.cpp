#include "gui/color-button.hpp"
#include <QPainter>
#include <QDebug>
#include <QScopedPointer>

ColorButton::ColorButton(QWidget* parent, const QColor& color)
    : QPushButton("", parent)
    , m_currentColor(color)
{
    emit changed(color);
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
    QObject::connect(this, SIGNAL(changed(QColor)), this, SLOT(onChanged(QColor)));
}

void ColorButton::onChanged(const QColor& color)
{
    m_currentColor = color;
    // Update style-sheet
    setStyleSheet(QString("background-color: %1").arg(color.name()));
}

void ColorButton::onClicked(bool)
{
    QScopedPointer<QColorDialog> dialog(new QColorDialog(parentWidget()));

    if (dialog->exec() != QDialog::Rejected)
        emit changed(dialog->selectedColor());
}

void ColorButton::setText(const QString&)
{
    // Changing text is prohibited.
}

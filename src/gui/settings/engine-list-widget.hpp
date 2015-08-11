#ifndef ENGINELISTWIDGET_HPP
#define ENGINELISTWIDGET_HPP

#include <QWidget>
#include <QListWidget>

namespace Ui {
class EngineListWidget;
}

class EngineListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EngineListWidget(QWidget *parent = 0);
    ~EngineListWidget();
public slots:
    void update();
private slots:
    void onAdd();
    void onRemove();
    void onConfigure();
private:
    Ui::EngineListWidget *ui;
};

#endif // ENGINELISTWIDGET_HPP

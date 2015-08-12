#ifndef ENGINE_LIST_WIDGET_HPP
#define ENGINE_LIST_WIDGET_HPP
#include <QListWidget>


class EngineListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit EngineListWidget(QWidget* parent = 0);
    ~EngineListWidget() = default;

    /*! \brief Returns selected settings instance */
    QString engineName() const;

    /*! \brief Tests whether there is something currently selected */
    bool selected() const;
public slots:
    void update();
private slots:
    void onClicked(QModelIndex);
private:
    QStringList m_engines;
    int m_currentId;
};

#endif // ENGINE_LIST_WIDGET_HPP

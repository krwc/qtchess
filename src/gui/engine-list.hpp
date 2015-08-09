#ifndef ENGINE_LIST_HPP
#define ENGINE_LIST_HPP
#include <QListWidget>
#include <QStringList>
#include <QDialog>

class EngineDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngineDialog(QWidget* parent = nullptr);
    /*! \brief Returns name of the engine */
    const QString& name() const;
private:
    QString m_name;
    QString m_path;
};

class EngineList : public QListWidget {
    Q_OBJECT
public:
    explicit EngineList(QWidget* parent = nullptr);
public slots:
    void update();
    void addEngine();
    void delEngine();
private:
    QStringList m_currentList;
};

#endif


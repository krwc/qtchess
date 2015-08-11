#ifndef ENGINE_WIDGET_HPP
#define ENGINE_WIDGET_HPP

#include <QWidget>
#include <QVector>
#include "engine/engine.hpp"
#include "game/board.hpp"
#include <memory>

namespace Ui {
class EngineWidget;
}

class Tree;
class EngineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EngineWidget(QWidget* parent = 0);
    ~EngineWidget();

    /*! \brief Overriden for view compactness. */
    virtual QSize sizeHint() const;
public slots:
    /*! \brief Sets current board position */
    void setBoard(const Board& board);
private slots:
    void onVariantParsed(VariantInfo);
    void onAnalyzeClicked();
    void onStopClicked();
    bool onSelectClicked();
public slots:
    void redraw();
private:
    void setEngine(QString name);

    Ui::EngineWidget *ui;
    Engine* m_engine;
    Board m_currentBoard;
    QVector<VariantInfo> m_variants;
};

#endif // ENGINE_WIDGET_HPP

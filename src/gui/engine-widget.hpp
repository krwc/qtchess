#ifndef ENGINE_WIDGET_HPP
#define ENGINE_WIDGET_HPP

#include <QWidget>
#include <QVector>
#include <QMutex>
#include "engine/engine.hpp"
#include "game/board.hpp"

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
    void onLineInfo(LineInfo);
    void onAnalyzeClicked();
    void onStopClicked();
public slots:
    void redraw();
private:
    Ui::EngineWidget *ui;
    Engine* m_engine;
    Board m_currentBoard;
    QVector<LineInfo> m_lines;
};

#endif // ENGINE_WIDGET_HPP

#ifndef GAME_TREE_WIDGET_HPP
#define GAME_TREE_WIDGET_HPP
#include "game-tree.hpp"
#include <QWidget>

class GameTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameTreeWidget(QWidget *parent = 0);

protected:
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
signals:

public slots:
private:
    GameTree mTree;
    // Currently selected move
    GameTreeNode* mCurrentMove;
};

#endif // GAME_TREE_WIDGET_HPP

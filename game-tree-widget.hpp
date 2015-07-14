#ifndef GAME_TREE_WIDGET_HPP
#define GAME_TREE_WIDGET_HPP
#include "game-tree.hpp"
#include <QWidget>
#include <QTextBrowser>
#include <QList>
#include <QMap>

class GameTreeWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit GameTreeWidget(QWidget *parent = 0);

    void addMove(Move move);
private slots:
    void onMoveClick(const QUrl& Link);

signals:
    void positionChanged(GameTreeNode*);
public slots:

private:
    // Game tree
    GameTree mTree;
    // Currently selected move
    GameTreeNode* mCurrentMove;
};

#endif // GAME_TREE_WIDGET_HPP

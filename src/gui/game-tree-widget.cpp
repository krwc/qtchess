#include "game-tree-widget.hpp"
#include "settings.hpp"
#include <QDebug>
#include <QPainter>
#include <QMenu>
#include <QContextMenuEvent>
// TODO: This should be part of the skin system.
static QString StyleSheet =
        "<style>\n"
        ".TreeBody { font-size: 14px; line-height: 140%; font-family: monospace; }\n"
        ".TreeMove { font-weight: bold; text-decoration: none; color: #000; }\n"
        "ul.TreeVariant { font-size: 12px; color: #0000FF; padding-left: 15px; list-style-type: none; }\n"
        "li.TreeVariant { }\n"
        ".TreeCurrentMove { background-color:#96c2cd; }\n"
        "</style>\n";

static QString BodyFormat = "<div class='TreeBody'>%1</div>";
static QString MoveFormat = "<a class='TreeMove' href='%1'>%2</a>";
static QString VariantLiFormat = "<li class='TreeVariant'>(%1)</li>";
static QString VariantUlFormat = "<ul class='TreeVariant'>%1</ul>";
static QString CurrentMoveFormat = "<span class='TreeCurrentMove'>%1</span>";


class TreeToHtmlConverter {
public:
    TreeToHtmlConverter(GameTree* Tree);
    QString html() const;
private:
    void traverse(QString& Result, GameTreeIterator It, int VariantDepth,
                  int HalfMoveNumber) const;
    GameTree* mTree;
};

TreeToHtmlConverter::TreeToHtmlConverter(GameTree* Tree)
  : mTree(Tree)
{

}

QString TreeToHtmlConverter::html() const {
    QString Result;
    traverse(Result, mTree->getIterator(), 0, 1);
    return StyleSheet + BodyFormat.arg(Result);
}

void TreeToHtmlConverter::traverse(QString &Ret, GameTreeIterator It, int VariantDepth,
                                   int HalfMoveNumber) const
{
    while (It.next()) {
        QString Hash = QString::number(size_t(It.getNode()));
        // TODO: Re-think and re-project this code.
        // We are in the node after the last move was played. This means
        // it is impossible to convert last move to algebraic notation, because
        // it requires previous game snapshot. This is why this ugly one step
        // in reverse direction is made. It should be designed better, I know.
        It.prev();
        QString MoveString = MoveFormat.arg(Hash, It.getNode()->Game.algebraicNotationString(It.getLastMove()));
        It.next();
        QString MoveId = QString::number((HalfMoveNumber + 1) / 2);
        QString MoveNumber;

        if (HalfMoveNumber % 2)
            MoveNumber = MoveId + ". ";
        else if (It.hasPrev()) {
            It.prev();
            if (It.hasChildren())
                MoveNumber = MoveId + "... ";
            It.next();
        }
        ++HalfMoveNumber;

        Ret += MoveNumber;
        if (It.getNode() == mTree->getLast())
            Ret += CurrentMoveFormat.arg(MoveString);
        else
            Ret += MoveString;
        Ret += ' ';

        QString ChildrenResult;
        for (auto& ChildIt: It.getChildren())
            traverse(ChildrenResult, ChildIt, VariantDepth+1, HalfMoveNumber);

        if (It.hasChildren())
            Ret += VariantUlFormat.arg(VariantLiFormat.arg(ChildrenResult));
    }
}

GameTreeWidget::GameTreeWidget(QWidget* parent)
    : QWebView(parent)
    , mTree(nullptr)
    , mHoveredNode(nullptr)
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    QObject::connect(&Settings::instance(), SIGNAL(changed()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(onMoveClick(QUrl)));
    QObject::connect(page(), SIGNAL(linkHovered(QString,QString,QString)), this,
                     SLOT(onLinkHover(QString)));
}

void GameTreeWidget::setGameTree(GameTree* Tree) {
    mTree = Tree;
    redraw();
}

void GameTreeWidget::redraw() {
    setHtml(TreeToHtmlConverter(mTree).html());
}

void GameTreeWidget::onMoveClick(const QUrl&) {
    emit positionSelected(mHoveredNode);
    redraw();
}

void GameTreeWidget::onLinkHover(const QString& Url) {
    if (Url.isEmpty())
        mHoveredNode = nullptr;
    else
        mHoveredNode = reinterpret_cast<GameTreeNode*>(Url.toULongLong());
}

void GameTreeWidget::contextMenuEvent(QContextMenuEvent* Event) {
    // Don't care if no node has been hovered.
    if (!mHoveredNode)
        return;

    // TODO: Make this work.
    QMenu Menu;
    Menu.addAction("Promote up");
    Menu.addAction("Promote to mainline");
    Menu.addSeparator();
    Menu.addAction("Delete branch from this move");

    Menu.exec(Event->globalPos());
    redraw();
}

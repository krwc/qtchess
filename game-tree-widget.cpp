#include "game-tree-widget.hpp"
#include "html-util.hpp"
#include <QDebug>
#include <QPainter>
#include <QStack>
#include <QQueue>

class TreeToHtmlConverter {
public:
    TreeToHtmlConverter(const GameTree& Tree)
      : mTree(Tree) {

    }

    QString html() const {
        QString Ret;
        /*
        QQueue<GameTreeNode*> Q;

        Q.enqueue(mTree.getRoot());
        int Depth = 0;

        while (!Q.isEmpty()) {
            GameTreeNode* Node = Q.dequeue();

            for (auto& Entry : Node->Next) {
                Ret += makeFullMove()
               /* Current = drawMove(Painter, Entry.first, Current);*
            }
        }*/
    }
private:
    QString makeMove(Move Move) {
        return "xD ";
    }

    QString makeFullMove(Move move, Move reply, int Depth) {
      /* Html::TagBuilder& Builder = Html::TagBuilder("div")
                .appendInnerText(QString().setNum(Depth) + ". ")
                .appendInnerText(makeUrl(makeMove(move), QString("#")));

        if (reply.From == Coord2D<int>(-1,-1))
            return Builder;
        else
            return Builder.appendInnerText(makeUrl(makeMove(reply)));*/
    }

    QString makeUrl(QString Text, QString Ref) {
        QString Style = Html::StyleSheetBuilder()
                .setFontWeight(Html::Bold)
                .setFontDecoration(Html::None)
                .setFontColor(Qt::GlobalColor::black);

        return Html::TagBuilder("a")
                .setProperty("href", Ref)
                .setProperty("style", Style)
                .appendInnerText(Text);
    }

private:
    const GameTree& mTree;
};

GameTreeWidget::GameTreeWidget(QWidget* parent)
    : QTextBrowser(parent)
    , mCurrentMove(nullptr)
{
    setOpenLinks(false);
    QObject::connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(onMoveClick(QUrl)));

}

void GameTreeWidget::addMove(Move move) {
    mCurrentMove = mTree.addVariation(mTree.getLast(), move);
    //emit setHtml(TreeToHtmlConverter(mTree).html());
}

void GameTreeWidget::onMoveClick(const QUrl& Link) {
    qDebug() << "Link clicked: " << Link;
}

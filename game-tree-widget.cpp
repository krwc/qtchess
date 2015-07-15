#include "game-tree-widget.hpp"
#include "html-util.hpp"
#include <QDebug>
#include <QPainter>
#include <QStack>
#include <QQueue>

class TreeToHtmlConverter {
public:
    TreeToHtmlConverter(const GameTree* Tree)
      : mTree(Tree) {

    }

    QString html() const {
        QString Ret;
        traverse(Ret, mTree->getIterator(), 0, 1, 0);
        return Ret;
    }
private:
    void traverse(QString& Ret, GameTreeIterator It, int SubvariationDepth,
                  int HalfMoveNumber, int MoveNumber) const {

        while (It.hasNext()) {
            It.next();
            // Half move is odd, it's a next full move.
            if (HalfMoveNumber % 2) {
                Ret += QString::number(MoveNumber+1) + ". ";
                MoveNumber++;
            }
            HalfMoveNumber++;

            Ret += makeUrl(It.getLastMove().toString(),
                           QString::number(size_t(It.getNode())));
            Ret += ' ';
        }
    }

    QString makeUrl(QString Text, QString Ref) const {
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
    const GameTree* mTree;
};

GameTreeWidget::GameTreeWidget(QWidget* parent)
    : QTextBrowser(parent)
    , mTree(nullptr)
{
    setOpenLinks(false);
    QObject::connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(onMoveClick(QUrl)));

}

void GameTreeWidget::setGameTree(GameTree* Tree) {
    mTree = Tree;
    redraw();
}

void GameTreeWidget::redraw() {
    emit setHtml(TreeToHtmlConverter(mTree).html());
}

void GameTreeWidget::onMoveClick(const QUrl& Link) {
    size_t NodeAddress = Link.toString(QUrl::None).toULongLong();
    emit positionSelected(reinterpret_cast<GameTreeNode*>(NodeAddress));
}

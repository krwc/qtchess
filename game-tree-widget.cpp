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
        traverse(Ret, mTree->getIterator(), 0, 1);
        return Ret;
    }
private:
    void traverse(QString& Ret, GameTreeIterator It, int SubvariationDepth,
                  int HalfMoveNumber) const {
        static const int SUB_OFFSET_PX = 5;

        while (It.hasNext()) {
            It.next();
            QString NodeHash = QString::number(size_t(It.getNode()));
            QString MoveStr = makeUrl(It.getLastMove().toString(), NodeHash);
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
            HalfMoveNumber++;

            Ret += MoveNumber + MoveStr;
            Ret += ' ';

            for (auto& ChildIt : It.getChildren()) {
                QString ChildRet;
                traverse(ChildRet, ChildIt, SubvariationDepth+1,
                         HalfMoveNumber);
                Ret += makeSubVariation(ChildRet);

            }
        }
    }

    QString makeSubVariation(QString Str) const {
        QString Style = Html::StyleSheetBuilder()
                .setFontDecoration(Html::None)
                .setFontColor(Qt::GlobalColor::blue);
        return Html::TagBuilder("p")
                .setProperty("style", Style)
                .appendInnerText('[' + Str + ']');
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

/*    QString makeSubVariation(int Offset) {
        QString Style = Html::StyleSheetBuilder()
                .setFontDecoration(Html::None)
                .setFontColor(Qt::GlobalColor::blue);

    }*/

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

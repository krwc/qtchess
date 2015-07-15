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
        qDebug() << Ret;
        return Html::TagBuilder("div")
                   .setProperty("style",
                       Html::StyleBuilder()
                           .setFontSizeInPx(14)
                           .setLineHeight(140)
                           .setFontFamily("monospace"))
                   .appendInnerText(Ret);
    }
private:
    void traverse(QString& Ret, GameTreeIterator It, int VariantDepth,
                  int HalfMoveNumber) const {
        static const int VARIANT_OFFSET_PX = 10;

        while (It.next()) {
            QString NodeHash = QString::number(size_t(It.getNode()));
            QString MoveStr = makeUrl(It.getLastMove().toString(), NodeHash);
            QString MoveId = QString::number((HalfMoveNumber + 1) / 2);
            QString MoveNumber;

            /* Non-even numbers indicate new full-move */
            if (HalfMoveNumber % 2)
                MoveNumber = MoveId + ". ";
            else if (It.hasPrev()) {
                It.prev();

                if (It.hasChildren())
                    MoveNumber = MoveId + "... ";

                It.next();
            }
            HalfMoveNumber++;

            Ret +=  MoveNumber + MoveStr;
            Ret += ' ';

            for (auto& ChildIt : It.getChildren()) {
                QString ChildRet;
                traverse(ChildRet, ChildIt, VariantDepth+1, HalfMoveNumber);

                Ret += makeVariant(ChildRet, VariantDepth * VARIANT_OFFSET_PX);
            }
        }
    }

    QString makeVariant(QString Str, int VariantOffset) const {
        return "<p style='margin-left:20px; line-height:inherit;'>(" + Str + ")</p>\n";
    }

    QString makeUrl(QString Text, QString Ref) const {
        Html::StyleBuilder StyleBuilder = Html::StyleBuilder()
                .setFontWeight(Html::Bold)
                .setFontDecoration(Html::None)
                .setFontColor(Qt::GlobalColor::black);

        return Html::TagBuilder("a")
                .setProperty("href", Ref)
                .setProperty("style", StyleBuilder)
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

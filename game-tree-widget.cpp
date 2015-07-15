#include "game-tree-widget.hpp"
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
        return QString("<div style='font-size:%1px; "
                       "line-height:%2%; font-family:monospace'>%3</div>")
                .arg("14", "160", Ret);
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

            Ret += MoveNumber;
            if (It.getNode() == ((GameTree*)mTree)->getLast())
                Ret += "<span style='background-color:#96c2cd;'>";
            Ret += MoveStr;
            if (It.getNode() == ((GameTree*)mTree)->getLast())
                Ret += "</span>";

            Ret += ' ';

            if (It.hasChildren())
                Ret += "<ul style='font-size:12px; color: #0000FF; padding-left: 10px; list-style-type: none;'>";
            for (auto& ChildIt : It.getChildren()) {
                QString ChildRet;
                traverse(ChildRet, ChildIt, VariantDepth+1, HalfMoveNumber);

                Ret += makeVariant(ChildRet, (VariantDepth+1) * VARIANT_OFFSET_PX);
            }
            if (It.hasChildren())
                Ret += "</ul>";
        }
    }

    QString makeVariant(QString Str, int VariantOffset) const {
        return QString("<li>(%1)</li>").arg(Str);
    }

    QString makeUrl(QString Text, QString Ref) const {
        return QString("<a href='%1' style='font-weight:bold; "
                       "text-decoration: none; color: #000'>%2</a>")
                .arg(Ref, Text);
    }
private:
    const GameTree* mTree;
};

GameTreeWidget::GameTreeWidget(QWidget* parent)
    : QWebView(parent)
    , mTree(nullptr)
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    QObject::connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(onMoveClick(QUrl)));
}

void GameTreeWidget::setGameTree(GameTree* Tree) {
    mTree = Tree;
    redraw();
}

void GameTreeWidget::redraw() {
    setHtml(TreeToHtmlConverter(mTree).html());
}

void GameTreeWidget::onMoveClick(const QUrl& Link) {
    size_t NodeAddress = Link.toString(QUrl::None).toULongLong();
    emit positionSelected(reinterpret_cast<GameTreeNode*>(NodeAddress));
    redraw();
}

#include "move-tree-widget.hpp"
#include "settings.hpp"
#include <QDebug>
#include <QPainter>
#include <QMenu>
#include <QStack>
#include <QContextMenuEvent>


static QString BodyFormat = "<div class='TreeBody'>%1</div>";
static QString MoveFormat = "<a class='TreeMove' href='%1'>%2</a>";
static QString VariantLiFormat = "<li class='TreeVariant'>(%1)</li>";
static QString VariantUlFormat = "<ul class='TreeVariant'>%1</ul>";
static QString CurrentMoveFormat = "<span class='TreeCurrentMove'>%1</span>";

static QString styleSheet = R"(
    <style>
        body {
            background-color: %5;
        }

        .TreeMoveNumber {
            color: %6;
        }

        .TreeBody {
            font-size: 14px;
            line-height: 140%;
            font-family: monospace;
        }

        .TreeMove {
            font-weight: bold;
            text-decoration: none;
            color: %1;
        }

        ul.TreeVariant {
            font-size: 12px;
            color: %2;
            padding-left: 15px;
            list-style-type: none;
        }

        li.TreeVariant { }

        .TreeCurrentMove {
            font-weight: bold;
            background-color: %3;
            color: %4;
        }
    </style>
)";
static QString numberFmt = "<span class='TreeMoveNumber'>%1</span>";
static QString moveFmt = "<a class='TreeMove' href='%1'>%2</a>";
static QString variantLiFmt = "<li class='TreeVariant'>(%1)</li>";
static QString variantUlFmt = "<ul class='TreeVariant'>%1</ul>";
static QString currentMoveFmt = "<span class='TreeCurrentMove'>%1</span>";

void TreeHtml::traverse(QString& result, Move lastMove, const TreeNode* node, const Tree* tree)
{
    if (lastMove != Move::NullMove) {
        const Board& board = node->parent()->board();
        QString prefix;
        QString notation = board.algebraicNotationString(lastMove);
        QString number = QString::number(board.fullMoveCount());
        QString uid = QString::number(node->uid());

        if (board.currentPlayer().isWhite())
            prefix.append(numberFmt.arg(number + ". "));
        else if (node->parent()->next() != node)
            prefix.append(numberFmt.arg(number + "... "));

        if (node->uid() == tree->currentNode()->uid())
            prefix.append(currentMoveFmt.arg(notation));
        else
            prefix.append(moveFmt.arg(uid, notation));
        result.append(prefix);
        result.append(" ");

        QString childResult;
        for (const Move& next : node->nonMainMoves())
            traverse(childResult, next, node->next(next), tree);
        if (!childResult.isNull())
            result.append(variantUlFmt.arg(variantLiFmt.arg(childResult)));
    }

    if (node->hasNeighbours())
        traverse(result, node->nextMove(), node->next(), tree);
}

QString TreeHtml::html(const Tree* tree)
{
    QString result;

    traverse(result, Move::NullMove, tree->rootNode(), tree);
    QString style = styleSheet.arg(
        Settings::instance().get(Settings::PgnMoveColor).value<QColor>().name(),
        Settings::instance().get(Settings::PgnVariationColor).value<QColor>().name(),
        Settings::instance().get(Settings::PgnHiColor).value<QColor>().name(),
        Settings::instance().get(Settings::PgnHiMoveColor).value<QColor>().name(),
        Settings::instance().get(Settings::PgnBackgroundColor).value<QColor>().name(),
        Settings::instance().get(Settings::PgnNumberColor).value<QColor>().name()
    );
    return style + QString("<div class='TreeBody'>%1</div>").arg(result);
}


MoveTreeWidget::MoveTreeWidget(QWidget* parent)
    : QWebView(parent)
    , m_tree(nullptr)
    , m_hoveredMoveUid(0)
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    QObject::connect(&Settings::instance(), &Settings::changed, this, &MoveTreeWidget::redraw);
    QObject::connect(this, &QWebView::linkClicked, this, &MoveTreeWidget::onMoveClicked);
    QObject::connect(page(), &QWebPage::linkHovered, this, &MoveTreeWidget::onMoveHovered);
}

void MoveTreeWidget::setTree(Tree* tree)
{
    m_tree = tree;
    redraw();
}

QSize MoveTreeWidget::sizeHint() const
{
    return QSize(250, 100);
}

void MoveTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    // Don't care if no node has been hovered.
    if (!m_hoveredMoveUid)
        return;

    // TODO: Make this work.
    QMenu menu;
    menu.addAction("Promote up");
    menu.addAction("Promote to mainline");
    menu.addSeparator();
    menu.addAction("Delete branch from this move");

    menu.exec(event->globalPos());
    redraw();
}


void MoveTreeWidget::redraw()
{
    setHtml(TreeHtml::html(m_tree));
}

void MoveTreeWidget::onMoveClicked(const QUrl&)
{
    emit moveSelected(m_hoveredMoveUid);
}

void MoveTreeWidget::onMoveHovered(const QString& url)
{
    if (url.isEmpty())
        m_hoveredMoveUid = 0;
    else
        m_hoveredMoveUid = url.toULongLong();
}

#endif

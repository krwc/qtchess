#include "gui/move-tree-widget.hpp"
#include "util/html-move-tree-builder.hpp"
#include "settings/settings-factory.hpp"
#include <QMenu>
#include <QContextMenuEvent>

void TreeHtml::traverse(HtmlMoveTreeBuilder& builder, Move lastMove,
                        const TreeNode* node, const Tree* tree)
{
    if (lastMove != Move::NullMove) {
        const Board& board = node->parent()->board();
        QString notation = board.algebraicNotationString(lastMove);
        QString number = QString::number(board.fullMoveCount());

        if (board.currentPlayer().isWhite())
            builder.addMoveNumber(number + ". ");
        // It is variant.
        else if (node->parent()->next() != node)
            builder.addMoveNumber(number + "... ");
        // It is position where we are starting from black move.
        else if (board.currentPlayer().isBlack() && !node->parent()->parent())
            builder.addMoveNumber(number + "... ");

        builder.addMove(notation, node->uid(), node->uid() == tree->currentNode()->uid());

        HtmlMoveTreeBuilder childBuilder;
        for (const Move& next : node->nonMainMoves())
            traverse(childBuilder, next, node->next(next), tree);

        if (!childBuilder.isEmpty())
            builder.addVariant(childBuilder.html());
    }

    if (node->hasNeighbours())
        traverse(builder, node->nextMove(), node->next(), tree);
}

QString TreeHtml::html(const Tree* tree)
{
    HtmlMoveTreeBuilder builder;
    traverse(builder, Move::NullMove, tree->rootNode(), tree);
    return builder.htmlWithStyle();
}


MoveTreeWidget::MoveTreeWidget(QWidget* parent)
    : QWebView(parent)
    , m_tree(nullptr)
    , m_hoveredMoveUid(0)
    , m_actionMoveUid(0)
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    QObject::connect(&SettingsFactory::html(), &HtmlSettings::changed,
                     this, &MoveTreeWidget::redraw);

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
    m_actionMoveUid = m_hoveredMoveUid;
    // Don't care if no node has been hovered.
    if (!m_actionMoveUid)
        return;

    QMenu menu;
    menu.addAction("Promote up", this, SLOT(onPromoteUp()));
    menu.addAction("Promote to mainline", this, SLOT(onPromoteToMainline()));
    menu.addSeparator();
    menu.addAction("Remove", this, SLOT(onRemove()));
    menu.exec(event->globalPos());

    redraw();
}

void MoveTreeWidget::redraw()
{
    setHtml(TreeHtml::html(m_tree));
}

void MoveTreeWidget::onPromoteUp()
{
    m_tree->promote(TreeNode::fromUid(m_actionMoveUid));
}

void MoveTreeWidget::onPromoteToMainline()
{

}

void MoveTreeWidget::onRemove()
{
    m_tree->remove(TreeNode::fromUid(m_actionMoveUid));
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

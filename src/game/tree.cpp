#include "tree.hpp"
#include <QDebug>

TreeNode::TreeNode(const Board& board, TreeNode* parent)
    : m_board(board)
    , m_parentLine(parent)
{

}

const Board& TreeNode::board() const
{
    return m_board;
}

const TreeNode* TreeNode::next() const
{
    return m_mainLine;
}

const TreeNode* TreeNode::next(Move move) const
{
    if (m_moves.contains(move))
        return m_moves[move];
    return nullptr;
}

const TreeNode* TreeNode::parent() const
{
    return m_parentLine;
}

bool TreeNode::hasNext(Move move) const
{
    return m_moves.contains(move);
}

bool TreeNode::hasNeighbours() const
{
    return m_moves.size();
}

Move TreeNode::nextMove() const
{
    for (const Move& move : m_moves.keys())
        if (m_moves[move] == m_mainLine)
            return move;
    return Move::NullMove;
}

QList<Move> TreeNode::nonMainMoves() const
{
    QList<Move> list = m_moves.keys();
    list.removeOne(nextMove());
    return list;
}

QList<Move> TreeNode::nextMoves() const
{
    return m_moves.keys();
}

size_t TreeNode::uid() const
{
    return reinterpret_cast<size_t>(this);
}

TreeNode* TreeNode::fromUid(size_t uid)
{
    return reinterpret_cast<TreeNode*>(uid);
}

void TreeNode::setMainLine(TreeNode* node)
{
    m_mainLine = node;
}

void TreeNode::addTransition(Move move, TreeNode* node)
{
    if (!hasNeighbours())
        setMainLine(node);

    m_moves[move] = node;
}

TreeNode* TreeNode::delTransition(Move move)
{
    TreeNode* node = m_moves[move];
    m_moves.remove(move);

    if (node == m_mainLine) {
        if (!hasNeighbours())
            setMainLine(nullptr);
        else
            setMainLine(m_moves.first());
    }

    return node;
}

Tree::Tree()
    : m_current(&m_root)
{
}

const TreeNode* Tree::rootNode() const
{
    return &m_root;
}

const TreeNode* Tree::currentNode() const
{
    return m_current;
}

bool Tree::addMove(Move move)
{
    if (!m_current->board().isLegal(move))
        return false;

    if (!m_current->hasNext(move)) {
        Board board = m_current->board();
        board.makeMove(move);

        TreeNode* node = new TreeNode(board, m_current);

        m_current->addTransition(move, node);
        m_current = node;
    } else
        // Next move exists. Just set it as a current move.
        m_current = const_cast<TreeNode*>(m_current->next(move));

    emit changed();

    return true;
}

bool Tree::delMove(Move move)
{
    if (!m_current->hasNext(move))
        return false;

    delete m_current->delTransition(move);

    emit changed();
    return true;
}

void Tree::clear()
{
    m_current = &m_root;

    for (const Move& move : m_root.nextMoves())
        delete m_root.delTransition(move);

    emit changed();
}

void Tree::setCurrent(TreeNode* node)
{
    m_current = node;
    emit changed();
}


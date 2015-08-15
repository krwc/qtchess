#include "tree.hpp"
#include <QDebug>

TreeNode::TreeNode(const Board& board, TreeNode* parent, TreeNode* parentLine)
    : m_board(board)
    , m_parent(parent)
    , m_parentLine(parentLine)
{

}

TreeNode::~TreeNode()
{
    for (TreeNode* next : m_moves.values())
        delete next;
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
    return m_parent;
}

const TreeNode* TreeNode::parentLine() const
{
    return m_parentLine;
}

const QString& TreeNode::annotation() const
{
    return m_annotation;
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

bool TreeNode::isChildNode(TreeNode* node) const
{
    if (m_moves.values().contains(node))
        return true;

    for (const Move& move : m_moves.keys())
        if (m_moves[move]->isChildNode(node))
            return true;
    return false;
}

void TreeNode::setBoard(const Board& board)
{
    m_board = board;
}

void TreeNode::setParentLine(TreeNode* node)
{
    m_parentLine = node;
}

void TreeNode::setParent(TreeNode* node)
{
    m_parent = node;
}

void TreeNode::setAnnotation(const QString& annotation)
{
    m_annotation = annotation;
}

TreeNode* TreeNode::next()
{
    return const_cast<TreeNode*>(const_cast<const TreeNode*>(this)->next());
}

TreeNode* TreeNode::next(Move move)
{
    return const_cast<TreeNode*>(const_cast<const TreeNode*>(this)->next(move));
}

TreeNode* TreeNode::parent()
{
    return const_cast<TreeNode*>(const_cast<const TreeNode*>(this)->parent());
}

TreeNode* TreeNode::parentLine()
{
    return const_cast<TreeNode*>(const_cast<const TreeNode*>(this)->parentLine());
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
        TreeNode* node;

        if (m_current->hasNeighbours())
            node = new TreeNode(board, m_current, m_current);
        else
            node = new TreeNode(board, m_current, const_cast<TreeNode*>(m_current->parentLine()));

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

    //emit changed();
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
    Q_ASSERT(node && "Setting null node.");

    m_current = node;
    emit changed();
}

void Tree::setRootBoard(const Board& board)
{
    clear();
    m_current->setBoard(board);
    emit changed();
}

void Tree::remove(TreeNode* node)
{
    if (node == &m_root)
        clear();
    else {
        TreeNode* current = m_current;
        TreeNode* parent = const_cast<TreeNode*>(node->parent());

        // If current was not a child node of the removed node, then
        // do not change m_current pointer to parent.
        if (!current->isChildNode(node))
            current = parent;

        m_current = parent;

        for (const Move& nextMove : parent->nextMoves()) {
            if (parent->next(nextMove) == node) {
                delMove(nextMove);
                break;
            }
        }
        // Restore m_current pointer
        m_current = current;

        emit changed();
    }
}

bool Tree::promote(TreeNode* node)
{
    Q_ASSERT(node && "Promoting null node.");

    // Lets assume it the tree looks like this and consider segments:
    // 1. d4 d5 2. Nf3 Nf6 3. c4 ( 3. b4 b5 4. a4 a5 ) Be7 4. Be2 O-O
    // [-----------1-----------] [---------2---------] [------3------]

    // Main line cannot be promoted.
    if (!node->parentLine())
        return false;

    // First element in the line containing a node. We can imagine this
    // by assuming that node points for example to 4. a4, then firstInLine
    // will point at 3.b4 as it is the first node in this variant line.
    TreeNode* firstInLine = node;
    TreeNode* lineParent;
    while (firstInLine->parent() != node->parentLine())
        firstInLine = firstInLine->parent();

    // Points to 2... Nf6
    lineParent = firstInLine->parent();

    // Update next moves. Next moves like 3. c4 Be7 4. Be2 O-O will be
    // demoted to a variant, and we have to remember their new parentLine.
    for (TreeNode* next = lineParent->next(); next; next = next->next())
        next->setParentLine(firstInLine->parentLine());

    // Now going back, (2) has to be updated so that every move from there
    // has parentLine equal to the (1) parent line, as this will be the new
    // main line.
    for (TreeNode* next = firstInLine; next; next = next->next())
        next->setParentLine(lineParent->parentLine());

    // The last thing that is left is to set lineParent main line, that is
    // to set 2... Nf6 main line to 3. b4.
    lineParent->setMainLine(firstInLine);

    emit changed();
    return true;
}

void Tree::promoteToMainline(TreeNode* node)
{
    while (promote(node));
}

void Tree::annotate(TreeNode* node, const QString& annotation)
{
    Q_ASSERT(node && "Annotated null node");

    node->setAnnotation(annotation);

    emit changed();
}

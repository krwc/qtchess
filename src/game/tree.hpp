#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP
#include <QObject>
#include <QMap>
#include "game/board.hpp"

/*! \brief Internal node structure representation */
class TreeNode {
public:
    TreeNode(const Board& board = Board(), TreeNode* parent = nullptr, bool isPv = true);

    /*! \brief Returns node current board */
    const Board& board() const;

    /*! \brief Returns next node in the mainline */
    const TreeNode* next() const;

    /*! \brief Returns pointer to the neighbour to which there is transition through given move */
    const TreeNode* next(Move move) const;

    /*! \brief Returns parent node */
    const TreeNode* parent() const;

    /*! \brief Checks whether \a move is one of the next moves. */
    bool hasNext(Move move) const;

    /*! \brief Tests whether this node has neighbours */
    bool hasNeighbours() const;

    /*! \brief Tests whether this node is not main variation */
    bool isPrincipalVariation() const;

    /*! \brief Returns next move in the mainline */
    Move nextMove() const;

    /*! \brief Returns list of next moves excluding main-line */
    QList<Move> nonMainMoves() const;

    /*! \brief Returns list of all next moves */
    QList<Move> nextMoves() const;

    /*! \brief Returns unique id of the node */
    size_t uid() const;

    /*! \brief Constructs node from unique id. If uid is invalid the behaviour is undefined. */
    static TreeNode* fromUid(size_t uid);

    /*! \brief Sets main line */
    void setMainLine(TreeNode* node);

    /*! \brief Adds new transition */
    void addTransition(Move move, TreeNode* node);

    /*! \brief Removes transition */
    TreeNode* delTransition(Move move);
private:
    /*!< true if this is node from principal variation */
    bool m_isPv;
    /*!< chess-board snapshot */
    Board m_board;
    /*!< main line in this line */
    TreeNode* m_mainLine = nullptr;
    /*!< parent line */
    TreeNode* m_parentLine = nullptr;
    /*!< all moves from this node */
    QMap<Move, TreeNode*> m_moves = {};
};

class Tree : public QObject
{
    Q_OBJECT
public:
    /*! \brief Constructs an empty tree */
    Tree();

    /*! \brief Returns current node */
    const TreeNode* currentNode() const;

    /*! \brief Returns root node */
    const TreeNode* rootNode() const;

    /*! \brief Adds new move to the current node
     * \return true if move is legal in current position
     */
    bool addMove(Move move);

    /*! \brief Deletes move from the current node
     * \return true if move is deleted
     */
    bool delMove(Move move);

    /*! \brief Clears the tree */
    void clear();

    /*! \brief Sets current node to be the given node */
    void setCurrent(TreeNode* node);
signals:
    /*! \brief Emited when tree is changed */
    void changed();
private:
    /*! \brief Root node */
    TreeNode m_root;
    /*! \brief Currently active node */
    TreeNode* m_current;
};


#endif

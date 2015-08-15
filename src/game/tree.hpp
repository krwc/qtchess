#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP
#include <QObject>
#include <QMap>
#include "game/board.hpp"

/*! \brief Internal node structure representation */
class Tree;
class TreeNode {
    friend class Tree;
public:
    ~TreeNode();

    TreeNode(const Board& board = Board(), TreeNode* parent = nullptr,
             TreeNode* parentLine = nullptr);

    /*! \brief Returns node current board */
    const Board& board() const;

    /*! \brief Returns next node in the mainline */
    const TreeNode* next() const;

    /*! \brief Returns pointer to the neighbour to which there is transition through given move */
    const TreeNode* next(Move move) const;

    /*! \brief Returns parent node */
    const TreeNode* parent() const;

    /*! \brief Returns parent line node */
    const TreeNode* parentLine() const;

    /*! \brief Returns node annotation. */
    const QString& annotation() const;

    /*! \brief Checks whether \a move is one of the next moves. */
    bool hasNext(Move move) const;

    /*! \brief Tests whether this node has neighbours */
    bool hasNeighbours() const;

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

    /*! \brief Checks whether given node is a child node of this node */
    bool isChildNode(TreeNode* node) const;

    /*! \brief Sets parent line pointer */
    void setParentLine(TreeNode* node);

    /*! \brief Sets parent pointer */
    void setParent(TreeNode* node);

    /*! \brief Sets annotation */
    void setAnnotation(const QString& annotation);
private:
    /*! \brief Sets current board */
    void setBoard(const Board& board);

    /** Same set of methods as above but for internal / friend class usage only,
     * to avoid stupid const_casting everywhere */
    TreeNode* next();
    TreeNode* next(Move move);
    TreeNode* parent();
    TreeNode* parentLine();
private:
    /*!< chess-board snapshot */
    Board m_board;
    /*!< main line in this line */
    TreeNode* m_mainLine = nullptr;
    /*!< parent node */
    TreeNode* m_parent = nullptr;
    /*!< parent line */
    TreeNode* m_parentLine = nullptr;
    /*!< all moves from this node */
    QMap<Move, TreeNode*> m_moves = {};
    /*!< Move annotation. */
    QString m_annotation;
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

    /*! \brief Sets current root board. And clears the tree. */
    void setRootBoard(const Board& board);

    /*! \brief Removes current node. */
    void remove(TreeNode* node);

    /*! \brief Promotes up given node if it is not already a main-line node.
     * \returns true if any promotion has occured, false otherwise
     */
    bool promote(TreeNode* node);

    /*! \brief Promotes line containing node to the main line */
    void promoteToMainline(TreeNode* node);

    /*! \brief Annotates given node */
    void annotate(TreeNode* node, const QString& annotation);
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

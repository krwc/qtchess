#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP
#include <vector>
#include <map>
#include "game-model.hpp"

struct GameTreeNode {
    ~GameTreeNode();
    // Model will represent current state, as it encapsulates position
    // and logic behind it.
    GameModel Game;
    // Parent node
    GameTreeNode* Parent;
    // Main line
    GameTreeNode* MainLine;
    // Next moves from the current node
    std::vector<std::pair<Move, GameTreeNode*>> Next;

    bool hasMove(Move move) const ;
};

class GameTreeIterator {
public:
    GameTreeIterator(const GameTreeNode* Root, Move StartMove);

    bool hasNext() const;
    bool hasPrev() const;
    bool hasChildren() const;
    bool next();
    bool prev();
    Move getLastMove() const;
    const GameTreeNode* getNode() const { return mCurrent; }
    std::vector<GameTreeIterator> getChildren() const;
private:
    // Move that leads to mCurrent
    Move mLastMove;
    const GameTreeNode* mRoot;
    const GameTreeNode* mCurrent;
};

class GameTree {
public:
    friend class GameTreeIterator;

    GameTree();
    ~GameTree();
    // Returns iterator
    GameTreeIterator getIterator() const;
    // Returns game root node
    GameTreeNode* getRoot();
    // Returns last node that was added
    GameTreeNode* getLast();
    // Adds edge with new move from the Current vertex
    GameTreeNode* addVariation(GameTreeNode* Current, Move Move);
    // Removes Current variation and its children recursively
    void delVariation(GameTreeNode* Root);

    void setLast(GameTreeNode* Node);
private:
    GameTreeNode mRoot;
    GameTreeNode* mLast;
};

#endif

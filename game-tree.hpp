#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP
#include <vector>
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
};

class GameTree {
public:


    GameTree();
    ~GameTree();
    // Returns game root node
    GameTreeNode* getRoot();
    // Returns last node that was added
    GameTreeNode* getLast();
    // Adds edge with new move from the Current vertex
    GameTreeNode* addVariation(GameTreeNode* Current, Move Move);
    // Removes Current variation and its children recursively
    void delVariation(GameTreeNode* Root);
private:
    GameTreeNode mRoot;
    GameTreeNode* mLast;
};

#endif

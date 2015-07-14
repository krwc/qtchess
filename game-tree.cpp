#include "game-tree.hpp"

GameTreeNode::~GameTreeNode() {
    for (const auto& It : Next)
        delete It.second;
}

GameTree::GameTree() {
    mRoot.Parent = nullptr;
    mRoot.MainLine = nullptr;
    mLast = nullptr;
}

GameTree::~GameTree() {

}

GameTreeNode* GameTree::getRoot() {
    return &mRoot;
}

GameTreeNode* GameTree::getLast() {
    if (!mLast)
        return getRoot();
    return mLast;
}

GameTreeNode* GameTree::addVariation(GameTreeNode* Current, Move Move) {
    GameTreeNode* NextNode = new GameTreeNode();
    NextNode->Parent = Current;
    NextNode->MainLine = NULL;
    NextNode->Game = Current->Game;
    NextNode->Game.makeMove(Move);

    Current->Next.emplace_back(Move, NextNode);

    if (Current->MainLine == NULL)
        Current->MainLine = NextNode;

    return NextNode;
}

void GameTree::delVariation(GameTreeNode* Root) {
    if (Root->Parent) {
        auto It = Root->Parent->Next.begin();
        // Delete move from parent
        while (It != Root->Parent->Next.end()) {
            if ((*It).second == Root)
                It = Root->Parent->Next.erase(It);
            else
                ++It;
        }
        // Check whether this was main line
        if (Root->Parent->MainLine == Root)
            Root->Parent->MainLine = NULL;
        delete Root;
    } else {
        // Otherwise an attempt to delete root node is made,
        // in such case we delete only its children
        for (auto& It : Root->Next)
            delVariation(It.second);
    }
}

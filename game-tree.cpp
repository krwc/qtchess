#include "game-tree.hpp"
#include <QDebug>

GameTreeNode::~GameTreeNode() {
    for (const auto& It : Next)
        delete It.second;
}

bool GameTreeNode::hasMove(Move move) const {
    for (auto& Entry : Next)
        if (Entry.first == move)
            return true;
    return false;
}

GameTreeIterator::GameTreeIterator(const GameTreeNode* Root, Move StartMove)
  : mLastMove(StartMove)
  , mRoot(Root)
  , mCurrent(Root)
{

}

bool GameTreeIterator::hasNext() const {
    return mCurrent->MainLine;
}

bool GameTreeIterator::hasPrev() const {
    return mCurrent->Parent;
}

bool GameTreeIterator::hasChildren() const {
    return mCurrent->Next.size() > 1;
}

bool GameTreeIterator::next() {
    if (!hasNext())
        return false;
    for (auto& Entry : mCurrent->Next)
        if (Entry.second == mCurrent->MainLine) {
            mLastMove = Entry.first;
            break;
        }
    ////mLastMove =
    mCurrent = mCurrent->MainLine;
    return true;
}

bool GameTreeIterator::prev() {
    if (!hasPrev())
        return false;
    mCurrent = mCurrent->Parent;
    return false;
}

Move GameTreeIterator::getLastMove() const {
    return mLastMove;
}

std::vector<GameTreeIterator> GameTreeIterator::getChildren() const {
    std::vector<GameTreeIterator> List;
    for (auto& Entry : mCurrent->Next) {
        if (Entry.second == mCurrent->MainLine)
            continue;
        List.push_back(GameTreeIterator(Entry.second, Entry.first));
    }
    return List;
}

GameTree::GameTree() {
    mRoot.Parent = nullptr;
    mRoot.MainLine = nullptr;
    mLast = nullptr;
}

GameTree::~GameTree() {

}

GameTreeIterator GameTree::getIterator() const {
    return GameTreeIterator(&mRoot, Move::NullMove);
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
    NextNode->MainLine = nullptr;
    NextNode->Game = Current->Game;
    NextNode->Game.makeMove(Move);

    Current->Next.emplace_back(Move, NextNode);

    if (Current->MainLine == nullptr)
        Current->MainLine = NextNode;

    mLast = NextNode;

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
        mLast = Root->Parent;
        delete Root;
    } else {
        // Otherwise an attempt to delete root node is made,
        // in such case we delete only its children
        for (auto& It : Root->Next)
            delVariation(It.second);
        mLast = getRoot();
    }
}

void GameTree::setLast(GameTreeNode* Node) {
    mLast = Node;
}

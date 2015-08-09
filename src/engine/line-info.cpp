#include "line-info.hpp"

LineInfo::LineInfo()
    : m_id(1)
    , m_score(0)
    , m_depth(0)
    , m_mate(0)
{
}

void LineInfo::setMoveList(const QStringList& moveList)
{
    m_moveList = moveList;
}

void LineInfo::setId(const int id)
{
    m_id = id;
}

void LineInfo::setMate(const int moves)
{
    m_mate = moves;
}

void LineInfo::setDepth(const int depth)
{
    m_depth = depth;
}

void LineInfo::setScore(const int score)
{
    m_score = score;
}

const QStringList& LineInfo::moveList() const
{
    return m_moveList;
}

int LineInfo::id() const
{
    return m_id;
}

int LineInfo::depth() const
{
    return m_depth;
}

int LineInfo::score() const
{
    return m_score;
}

int LineInfo::mate() const
{
    return m_mate;
}


#include "engine/variant-info.hpp"

VariantInfo::VariantInfo()
    : m_id(1)
    , m_score(0)
    , m_depth(0)
    , m_mate(0)
{
}

void VariantInfo::setMoveList(const QStringList& moveList)
{
    m_moveList = moveList;
}

void VariantInfo::setId(const int id)
{
    m_id = id;
}

void VariantInfo::setMate(const int moves)
{
    m_mate = moves;
}

void VariantInfo::setDepth(const int depth)
{
    m_depth = depth;
}

void VariantInfo::setScore(const int score)
{
    m_score = score;
}

const QStringList& VariantInfo::moveList() const
{
    return m_moveList;
}

int VariantInfo::id() const
{
    return m_id;
}

int VariantInfo::depth() const
{
    return m_depth;
}

int VariantInfo::score() const
{
    return m_score;
}

int VariantInfo::mate() const
{
    return m_mate;
}


#ifndef VARIANT_INFO_HPP
#define VARIANT_INFO_HPP
#include <QStringList>

class VariantInfo {
public:
    VariantInfo();

    void setMoveList(const QStringList& moveList);
    void setId(const int id);
    void setMate(const int moves);
    void setScore(const int score);
    void setDepth(const int depth);
    void setNps(const int nodesPerSecond);

    const QStringList& moveList() const;
    int id() const;
    int score() const;
    int mate() const;
    int depth() const;
    int nps() const;
private:
    QStringList m_moveList;
    int m_id;
    int m_score;
    int m_depth;
    int m_mate;
    int m_nps;
};

#endif

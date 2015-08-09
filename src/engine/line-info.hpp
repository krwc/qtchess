#ifndef LINEINFO_HPP
#define LINEINFO_HPP
#include <QStringList>

class LineInfo {
public:
    LineInfo();

    void setMoveList(const QStringList& moveList);
    void setId(const int id);
    void setMate(const int moves);
    void setScore(const int score);
    void setDepth(const int depth);

    const QStringList& moveList() const;
    int id() const;
    int score() const;
    int mate() const;
    int depth() const;
private:
    QStringList m_moveList;
    int m_id;
    int m_score;
    int m_depth;
    int m_mate;
};

#endif // LINEINFO_HPP

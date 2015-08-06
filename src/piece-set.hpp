#ifndef PIECESET_HPP
#define PIECESET_HPP
#include <game/pieces.hpp>
#include <game/board.hpp>
#include <QString>
#include <QtSvg/QSvgRenderer>
#include <QPixmap>
#include <QDebug>
#include <map>

class PieceSet
{
public:
    // Returns list of available sets names
    static QStringList getAvailableSets();
public:
    PieceSet(QString StyleName);
    ~PieceSet();
    PieceSet(const PieceSet&) = delete;
    PieceSet& operator=(const PieceSet&) = delete;

    /* Returns ready to use svg piece renderer */
    QSvgRenderer& getPieceRenderer(Piece piece, Player Owner) {
        return *mPieceRenderers[std::make_pair(piece.type(), Owner)];
    }
    /* Returns pixmap of given size. */
    QPixmap& getPiecePixmap(Piece piece, int size);
    /* Returns piece set style name */
    QString styleName() const;
private:
    QString mStyleName;
    std::map<std::pair<Piece::Type, Player>, QSvgRenderer*> mPieceRenderers;
    /* Cached, prerendered pixmaps */
    std::map<QSvgRenderer*, QPixmap*> mPixmap;
    /* Last prerendered sizes */
    std::map<QSvgRenderer*, int> mPixmapSize;
};

#endif // PIECESET_HPP

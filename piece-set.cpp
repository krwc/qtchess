#include "piece-set.hpp"
#include <QString>
#include <QPainter>
#include <algorithm>
#include <iostream>

static const QString PieceAssetsPathPrefix = "assets/pieces";
static const QString PieceName[] = {
    [PIECE_PAWN]    = "pawn",
    [PIECE_KNIGHT]  = "knight",
    [PIECE_BISHOP]  = "bishop",
    [PIECE_ROOK]    = "rook",
    [PIECE_KING]    = "king",
    [PIECE_QUEEN]   = "queen"
};

static QSvgRenderer* MakeSvgRenderer(std::pair<Piece, Player> Kind, QString Style) {
    QString Path = PieceAssetsPathPrefix + "/" + Style + "/";

    if (Kind.second == PLAYER_WHITE)
        Path += "white_";
    else
        Path += "black_";

    Path += PieceName[Kind.first];
    Path += ".svg";

    return new QSvgRenderer(Path);
}

static QPixmap* RenderPiece(QSvgRenderer* R, int Size) {
    QPixmap* Pixmap = new QPixmap(Size, Size);
    Pixmap->fill(QColor(0, 0, 0, 0));
    QPainter Painter(Pixmap);
    QRect Dest(0, 0, Size, Size);
    R->render(&Painter, Dest);
    return Pixmap;
}

PieceSet::PieceSet(QString PieceStyleName)
{
    for (int i = 0; i < 6; i++) {
        auto white = std::make_pair(Piece(i), PLAYER_WHITE);
        auto black = std::make_pair(Piece(i), PLAYER_BLACK);
        mPieceRenderers[white] = MakeSvgRenderer(white, PieceStyleName);
        mPieceRenderers[black] = MakeSvgRenderer(black, PieceStyleName);
    }
}

PieceSet::~PieceSet()
{
    // Free renderers
    for (auto entry : mPieceRenderers) {
        std::cout << "Attempt to delete " << entry.second << std::endl;
        delete entry.second;
    }
    // Free pixmaps too
    for (auto entry : mPixmap) {
        std::cout << "Attempt to delete " << entry.second << std::endl;
        delete entry.second;
    }
    std::cout << std::endl;
}

QPixmap& PieceSet::getPiecePixmap(Piece Piece, Player Owner, int Size) {
    QSvgRenderer* R = &getPieceRenderer(Piece, Owner);

    if (mPixmapSize[R] != Size) {
        mPixmapSize[R] = Size;
        delete mPixmap[R];
        mPixmap[R] = RenderPiece(R, Size);
    }
    return *mPixmap[R];
}



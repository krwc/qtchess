#include "piece-set.hpp"
#include <QString>
#include <QPainter>
#include <QDir>
#include <algorithm>
#include <QDebug>

static const QString PieceAssetsPathPrefix = "assets/pieces";
static const QString PieceName[] = {
    [Piece::Pawn]    = "pawn",
    [Piece::Knight]  = "knight",
    [Piece::Bishop]  = "bishop",
    [Piece::Rook]    = "rook",
    [Piece::King]    = "king",
    [Piece::Queen]   = "queen"
};

static QSvgRenderer* MakeSvgRenderer(std::pair<Piece::Type, Player> Kind, QString Style) {
    QString Path = PieceAssetsPathPrefix + "/" + Style + "/";

    if (Kind.second.isWhite())
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

QStringList PieceSet::getAvailableSets() {
    return QDir(PieceAssetsPathPrefix).entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
}

PieceSet::PieceSet(QString PieceStyleName)
  : mStyleName(PieceStyleName)
{
    for (int i = 0; i < 6; i++) {
        auto white = std::make_pair(Piece::Type(i), Player::white());
        auto black = std::make_pair(Piece::Type(i), Player::black());
        mPieceRenderers[white] = MakeSvgRenderer(white, PieceStyleName);
        mPieceRenderers[black] = MakeSvgRenderer(black, PieceStyleName);
    }
}

PieceSet::~PieceSet()
{
    // Free renderers
    for (auto entry : mPieceRenderers)
        delete entry.second;
    // Free pixmaps too
    for (auto entry : mPixmap)
        delete entry.second;
}

QPixmap& PieceSet::getPiecePixmap(Piece piece, int size) {
    QSvgRenderer* R = &getPieceRenderer(piece, piece.owner());

    if (mPixmapSize[R] != size) {
        mPixmapSize[R] = size;
        delete mPixmap[R];
        mPixmap[R] = RenderPiece(R, size);
    }
    return *mPixmap[R];
}

QString PieceSet::styleName() const {
    return mStyleName;
}

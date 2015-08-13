#include "pieces.hpp"

Piece::Piece(QChar symbol)
{
    bool isBlack = symbol.isLower();
    QChar lower = symbol.toLower();

    if (lower == 'p')
        mType = Piece::Type::Pawn;
    else if (lower == 'n')
        mType = Piece::Type::Knight;
    else if (lower == 'b')
        mType = Piece::Type::Bishop;
    else if (lower == 'r')
        mType = Piece::Type::Rook;
    else if (lower == 'q')
        mType = Piece::Type::Queen;
    else if (lower == 'k')
        mType = Piece::Type::King;
    else
        mType = Piece::Type::None;

    if (mType == Piece::Type::None)
        mOwner = Player::none();
    else
        mOwner = isBlack ? Player::black() : Player::white();
}

Piece::Piece(Piece::Type type, Player owner)
    : mType(type)
    , mOwner(owner)
{
}

QString Piece::symbolString() const
{
    switch (mType) {
    case Piece::Pawn:   return "P";
    case Piece::Knight: return "N";
    case Piece::Bishop: return "B";
    case Piece::Rook:   return "R";
    case Piece::Queen:  return "Q";
    case Piece::King:   return "K";
    default: return "";
    }
}

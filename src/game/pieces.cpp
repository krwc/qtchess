#include "pieces.hpp"

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

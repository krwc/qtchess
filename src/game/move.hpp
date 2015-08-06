#ifndef MOVE_HPP
#define MOVE_HPP
#include "common.hpp"
#include "pieces.hpp"

struct Move {
    // Null move
    static const Move NullMove;

    Coord2D<int> From;
    Coord2D<int> To;
    Piece::Type PromotionPiece;

    Move() {}
    Move(Coord2D<int> From, Coord2D<int> To, Piece::Type PromotionTo = Piece::None)
      : From(From)
      , To(To)
      , PromotionPiece(PromotionTo)
    { }

    bool operator== (const Move& Move) const {
        return Move.From == From && Move.To == To &&
               Move.PromotionPiece == PromotionPiece;
    }

    Coord2D<int> from() const {
        return From;
    }

    Coord2D<int> to() const {
        return To;
    }

    Piece::Type promotionPiece() const {
        return PromotionPiece;
    }
};



#endif // MOVE_HPP


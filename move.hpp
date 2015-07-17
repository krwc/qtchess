#ifndef MOVE_HPP
#define MOVE_HPP
#include "common.hpp"
#include "pieces.hpp"

struct Move {
    // Null move
    static const Move NullMove;

    Coord2D<int> From;
    Coord2D<int> To;
    Piece PromotionPiece;

    Move() {}
    Move(Coord2D<int> From, Coord2D<int> To, Piece PromotionTo = PIECE_QUEEN)
      : From(From)
      , To(To)
      , PromotionPiece(PromotionTo)
    { }

    bool operator== (const Move& Move) const {
        return Move.From == From && Move.To == To &&
               Move.PromotionPiece == PromotionPiece;
    }
};



#endif // MOVE_HPP


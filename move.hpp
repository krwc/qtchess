#ifndef MOVE_HPP
#define MOVE_HPP
#include "common.hpp"
#include "pieces.hpp"

struct Move {
    Coord2D<int> From;
    Coord2D<int> To;
    Piece PromotionPiece;

    Move() {}
    Move(Coord2D<int> From, Coord2D<int> To, Piece PromotionTo = PIECE_QUEEN)
      : From(From)
      , To(To)
      , PromotionPiece(PromotionTo)
    { }
};

#endif // MOVE_HPP


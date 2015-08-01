#ifndef PIECES_HPP
#define PIECES_HPP
#include "game/player.hpp"
#include "common.hpp"

enum Piece {
    PIECE_NONE   = -1, /* empty piece */
    PIECE_PAWN   = 0,
    PIECE_KNIGHT = 1,
    PIECE_BISHOP = 2,
    PIECE_ROOK   = 3,
    PIECE_KING   = 4,
    PIECE_QUEEN  = 5
};

#endif

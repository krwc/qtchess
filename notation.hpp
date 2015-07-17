#ifndef NOTATION_HPP
#define NOTATION_HPP
#include <QString>
#include "game-model.hpp"

namespace Notation {
    // Converts move to basic notation string.
    QString moveToBasicNotation(Move);
    // Converts move to algebraic notation.
    QString moveToAlgebraicNotation(const GameModel&, Move);
}

#endif // NOTATION_HPP


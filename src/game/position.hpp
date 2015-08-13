#ifndef POSITION_HPP
#define POSITION_HPP
#include "game/pieces.hpp"
#include <vector>

class Position {
public:
    /*! \brief initializes position to the default position */
    Position();

    /*! \brief returns piece at given rank, file */
    const Piece& pieceAt(int file, int rank) const;
    
    /*! \brief sets piece at given rank, file */
    void setPieceAt(int file, int rank, Piece piece);

    /*! \brief constructs default position */
    static Position defaultPosition();

    /*! \brief constructs empty position */
    static Position emptyPosition();
private:
    Piece mSquares[8][8];
};

#endif

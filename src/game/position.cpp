#include "game/position.hpp"

Position::Position() 
{
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        setPieceAt(i, j, Piece());
}

const Piece& Position::pieceAt(int file, int rank) const {
    return mSquares[rank][file];
}

void Position::setPieceAt(int file, int rank, Piece piece) {
    mSquares[rank][file] = piece;
}

Position Position::defaultPosition() {
    static Piece::Type piecesRank[] = {
        Piece::Type::Rook,
        Piece::Type::Knight,
        Piece::Type::Bishop,
        Piece::Type::Queen,
        Piece::Type::King,
        Piece::Type::Bishop,
        Piece::Type::Knight,
        Piece::Type::Rook
    };
    Position position;
    
    for (int file = 0; file < 8; file++) {
        position.setPieceAt(file, 0, Piece(piecesRank[file], Player::black()));
        position.setPieceAt(file, 1, Piece(Piece::Type::Pawn, Player::black()));
        position.setPieceAt(file, 6, Piece(Piece::Type::Pawn, Player::white()));
        position.setPieceAt(file, 7, Piece(piecesRank[file], Player::white()));
    }

    return position;
}

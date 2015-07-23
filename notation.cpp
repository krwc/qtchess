#include "notation.hpp"

namespace Notation {

QString File(int file) {
    return QString(char(file + 'a'));
}

QString Rank(int rank) {
    return QString(char(7 - rank + '1'));
}

QString PieceString(Piece Piece) {
    switch (Piece) {
    case PIECE_KNIGHT: return "N";
    case PIECE_BISHOP: return "B";
    case PIECE_ROOK:   return "R";
    case PIECE_QUEEN:  return "Q";
    case PIECE_KING:   return "K";
    default:
        return "";
    }
}

QString moveToBasicNotation(Move Move)
{
    return QString(Move.From.x + 'a') + QString(7-Move.From.y + '1') +
           QString(Move.To.x + 'a') + QString(7-Move.To.y + '1');
}

QString moveToAlgebraicNotation(const GameModel& Model, Move Move) {
    Piece Piece = Model.getField(Move.From).Piece;
    MoveType Type;
    GameState State;
    QString FileFrom = File(Move.From.x);
    QString RankFrom = Rank(Move.From.y);
    QString FileTo = File(Move.To.x);
    QString RankTo = Rank(Move.To.y);

    // Not interested in illegal moves
    if (!Model.isLegal(Move, &State, &Type))
        return "";
    QString Check = State.IsCheck ? "+" : "";

    switch (Type) {
    case MOVE_CASTLE_SHORT:
        return "O-O" + Check;
    case MOVE_CASTLE_LONG:
        return "O-O-O" + Check;
    case MOVE_ENPASSANT_CAPTURE:
        return FileFrom + "x" + File(Move.To.x) + Rank(Move.From.y) + Check;
    case MOVE_PROMOTION:
        return FileTo + RankTo + "=" + PieceString(Move.PromotionPiece) + Check;
    default:
        // Move is not exactly interesting.
        break;
    }

    /* TODO: Avoid move ambiguity for pieces in the same file, rank, and diagonal and so on. */
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

        }
    }


    // Still it might be a capture.
    if (Model.getField(Move.To).Piece != PIECE_NONE)
        return (Piece == PIECE_PAWN ? FileFrom : PieceString(Piece)) + "x" + FileTo + RankTo + Check;

    // Normal move
    return PieceString(Piece) + FileTo + RankTo + Check;
}

}

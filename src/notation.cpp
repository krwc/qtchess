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
    return "";
}

QString moveToBasicNotation(Move Move)
{
    return QString(Move.From.x + 'a') + QString(7-Move.From.y + '1') +
           QString(Move.To.x + 'a') + QString(7-Move.To.y + '1');
}

QString moveToAlgebraicNotation(const GameModel& Model, Move Move) {
    Piece MovedPiece = Model.piece(Move.From);
    Player PieceOwner = Model.owner(Move.From);
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
        break;
    }

    QString Uniqueness;

    /* Avoid move ambiguity */
    if (MovedPiece != PIECE_KING && MovedPiece != PIECE_PAWN) {
        CoordsVector Attackers;

        for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Coord2D<int> Current(file, rank);
            if (Current == Move.From)
                continue;

            if (Model.piece(Current) == MovedPiece &&
                Model.owner(Current) == PieceOwner) {
                if (Contains(Model.getAttackedCoords(MovedPiece, PieceOwner, Current), Move.To))
                    Attackers.push_back(Current);
            }
        }}
        int AttackersInSameFile = 0;
        int AttackersInSameRank = 0;

        for (auto& Item : Attackers) {
            if (Item.x == Move.From.x)
                ++AttackersInSameFile;
            if (Item.y == Move.From.y)
                ++AttackersInSameRank;
        }

        if (AttackersInSameRank)
            Uniqueness += FileFrom;
        if (AttackersInSameFile)
            Uniqueness += RankFrom;

        if (!AttackersInSameRank && !AttackersInSameFile && Attackers.size())
            Uniqueness += FileFrom;
    }

    // Still it might be a capture.
    if (Model.piece(Move.To) != PIECE_NONE)
        return (MovedPiece == PIECE_PAWN ? FileFrom : PieceString(MovedPiece)) + Uniqueness + "x" + FileTo + RankTo + Check;

    // Normal move
    return PieceString(MovedPiece) + Uniqueness + FileTo + RankTo + Check;
}

}

#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP
#include "pieces.hpp"
#include "move.hpp"
#include "player.hpp"
#include <map>
#include <vector>
#include <cassert>

enum MoveType {
    MOVE_NONSPECIAL,
    MOVE_CASTLE_SHORT,
    MOVE_CASTLE_LONG,
    MOVE_ENPASSANT_GENERATE,
    MOVE_ENPASSANT_CAPTURE,
    MOVE_PROMOTION
};

struct GameState {
    std::map<Player, bool> ShortCastlingRight;
    std::map<Player, bool> LongCastlingRight;
    bool IsCheck;
    Player WhoIsPlaying;
    // Player who can do an en-passant capture
    Player EnPassantPlayer;
    // Coordinates of the field that will be a capturer destination
    Coord2D<int> EnPassantCoords;
};

struct Field {
    enum Piece Piece;
    Player Owner;
};

class GameModel {
public:
    static const GameState InitialGameState;
    static const Field InitialGamePosition[8][8];
    static const Coord2D<int> A1, B1, C1, D1, F1, G1, H1;
    static const Coord2D<int> A8, B8, C8, D8, F8, G8, H8;

    GameModel();

    Player owner(int column, int row) const { return mFields[row][column].Owner; }
    Piece piece(int column, int row) const { return mFields[row][column].Piece; }

    Player owner(Coord2D<int> coord) const { return owner(coord.x, coord.y); }
    Piece piece(Coord2D<int> coord) const { return piece(coord.x, coord.y); }

    /* Checks if move is legal, passed GameState and MoveType are optional,
       and whey they're passed function sets state to after-the-move state
       and type to one of the MoveType values */
    bool isLegal(Move move, GameState* RetState, MoveType* Type) const;
    /* Makes the move */
    bool makeMove(Move move);
    /* Tests if current position is a check for a current side */
    bool isCheck() const;
    /* Tests if current position is a check-mate */
    bool isCheckmate() const;
    /** Returns true if given player has short castling rights */
    bool hasShortCastlingRights(Player player) const;
    /** Returns true if given player has long castling rights */
    bool hasLongCastlingRights(Player player) const;

    /** Returns list of attacked coordinates by given piece owned by owner on given position. */
    CoordsVector getAttackedCoords(Piece piece, Player owner, Coord2D<int> position) const;

    Player getCurrentPlayer() const { return mState.WhoIsPlaying; }

private:
    CoordsVector getPawnAttack(int x, int y, Player Owner) const;
    CoordsVector getBishopAttack(int x, int y) const;
    CoordsVector getKnightAttack(int x, int y) const;
    CoordsVector getRookAttack(int x, int y) const;
    CoordsVector getQueenAttack(int x, int y) const;
    CoordsVector getKingAttack(int x, int y) const;

    void movePieces(Move move, MoveType Type);
    /* Tests whether given player is in check after given move */
    bool isInCheckAfterTheMove(Player victim, Move move, MoveType type) const;
    /* Check unaware functions, they just test if move has right "geometry" */
    bool isLegalPawnMove(Move move, MoveType& Special, Coord2D<int>& EnPassantCoords) const;
    bool isLegalKnightMove(Move move) const;
    bool isLegalBishopMove(Move move) const;
    bool isLegalRookMove(Move move) const;
    bool isLegalQueenMove(Move move) const;
    bool isLegalKingMove(Move move, bool& MoveIsCastle, MoveType& Side) const;
    bool isLegalCoord(Coord2D<int> Coord) const;
    bool isLegalCoord(int x, int y) const;
    bool canCastle(MoveType castleType) const;
    int countAttacksFor(Coord2D<int> coord, Player attacker) const;
    int countChecksFor(Player player) const;
private:

    GameState mState;
    /* Some explanation about representation and field coordinates
       is mandatory at this point. It will be assumed everywhere else
       that this array represents board in a way pictured below, that is:

            0     1      ...     7

       0   A8 --- B8 --- ... --- H8
            |     |       |      |
            |     |       |      |
       1   A7 --- B7 --- ... --- H7
       .    .     .       .      .
       .    .     .       .      .
       .    .     .       .      .
       7   A1 --- B1 --- ... --- H1

        So A8 is field (0, 0), and H1 is (7, 7). This couldn't be simpler. */
    mutable Field mFields[8][8];
};

#endif // GAME_MODEL_HPP

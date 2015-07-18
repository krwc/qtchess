#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP
#include "pieces.hpp"
#include "move.hpp"
#include <vector>
#include <cassert>

enum Player {
    PLAYER_NONE = -1, /* empty player "seat" */
    PLAYER_WHITE = 0,
    PLAYER_BLACK = 1
};

enum MoveType {
    MOVE_NONSPECIAL,
    MOVE_CASTLE_SHORT,
    MOVE_CASTLE_LONG,
    MOVE_ENPASSANT_GENERATE,
    MOVE_ENPASSANT_CAPTURE,
    MOVE_PROMOTION
};

struct GameState {
    bool ShortCastlingRight[2];
    bool LongCastlingRight[2];
    bool IsCheck;
    Player WhoIsPlaying;
    // Player who can do an en-passant capture
    Player EnPassantPlayer;
    // Coordinates of the field that will be a capturer destination
    Coord2D<int> EnPassantCoords;
};

struct Field {
    enum Piece Piece;
    enum Player Owner;
};

class GameModel {
public:
    static const GameState InitialGameState;
    static const Field InitialGamePosition[8][8];
    static const Coord2D<int> A1, B1, C1, D1, F1, G1, H1;
    static const Coord2D<int> A8, B8, C8, D8, F8, G8, H8;

    GameModel();

    const Field& getField(int column, int row) const {
        assert(row < 8 && column < 8 && row >= 0 && column >= 0);
        return mFields[row][column];
    }
    const Field& getField(Coord2D<int> Coord) const {
        return getField(Coord.x, Coord.y);
    }
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
private:
    void movePieces(Move move, MoveType Type);
    /* Test whether after given move passed Player is in check */
    bool isCheckAfterTheMove(Player Player, Move move, MoveType Type) const;
    /* Check unaware functions, they just test if move has right "geometry" */
    bool isLegalPawnMove(Move move, MoveType& Special, Coord2D<int>& EnPassantCoords) const;
    bool isLegalKnightMove(Move move) const;
    bool isLegalBishopMove(Move move) const;
    bool isLegalRookMove(Move move) const;
    bool isLegalQueenMove(Move move) const;
    bool isLegalKingMove(Move move, bool& MoveIsCastle, MoveType& Side) const;
    bool isLegalCoord(Coord2D<int> Coord) const;
    bool isLegalCoord(int x, int y) const;
    bool canCastle(MoveType CastleType) const;
    int countAttacksFor(Coord2D<int> Coord, Player Attacker) const;
    int countChecksFor(Player player) const;

    std::vector<Coord2D<int>> getPawnAttack(int x, int y, Player Owner) const;
    std::vector<Coord2D<int>> getBishopAttack(int x, int y) const;
    std::vector<Coord2D<int>> getKnightAttack(int x, int y) const;
    std::vector<Coord2D<int>> getRookAttack(int x, int y) const;
    std::vector<Coord2D<int>> getQueenAttack(int x, int y) const;
    std::vector<Coord2D<int>> getKingAttack(int x, int y) const;


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

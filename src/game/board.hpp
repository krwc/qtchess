#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP
#include "position.hpp"
#include "move.hpp"
#include "player.hpp"
#include <map>
#include <vector>
#include <cassert>
#include <QString>

// FIXME: This enumeration is ugly as hell, move specifics should be stored differently.
enum MoveType {
    MOVE_NONSPECIAL,
    MOVE_CASTLE_SHORT,
    MOVE_CASTLE_LONG,
    MOVE_ENPASSANT_GENERATE,
    MOVE_ENPASSANT_CAPTURE,
    MOVE_PROMOTION
};

// FIXME: This structure should not be public.
struct GameState {
    std::map<Player, bool> ShortCastlingRight;
    std::map<Player, bool> LongCastlingRight;
    bool IsCheck;
    Player WhoIsPlaying;
    // Player who can do an en-passant capture
    Player EnPassantPlayer;
    // Coordinates of the field that will be a capturer destination
    Coord2D<int> EnPassantCoords;
    // Number of half-moves since the last capture or pawn move
    int HalfMoveClock;
    // Number of full-moves
    int FullMoveCounter;
};

class Board {
public:
    static const GameState InitialGameState;
    static const Coord2D<int> A1, B1, C1, D1, F1, G1, H1;
    static const Coord2D<int> A8, B8, C8, D8, F8, G8, H8;

    Board();

    /*! \brief Sets board position from fen if it is valid.
     * \returns true if fen position is valid, false otherwise
     */
    bool setFen(QString fen);

    /*! \brief Returns FEN representation of current position */
    QString toFen() const;

    /*! \brief Returns file string */
    QString fileString(int file) const;

    /*! \brief Returns rank string */
    QString rankString(int rank) const;

    /*! \brief Returns rank code */
    int charToRank(QChar rank) const;

    /*! \brief Returns rank code */
    int charToFile(QChar file) const;

    /*! \brief Returns field string */
    QString squareString(Coord2D<int> coord) const;

    /*! \brief Returns long algebraic notation of the given move */
    QString longAlgebraicNotationString(Move move) const;

    /*! \brief Returns algebraic notation string of the given move */
    QString algebraicNotationString(Move move) const;

    /*! \brief Returns move from long algebraic notation */
    Move longAlgebraicNotationToMove(const QString& lan) const;

    /*! \brief Returns full move count */
    int fullMoveCount() const;

    /*! \brief Gets piece reference.
     *
     * Returns piece reference located at given coordinate;
     * if coordinate happens to be out-of-range an assertion
     * will be thrown.
     *
     * \return Piece reference located at given coordinate
     */
    const Piece& pieceAt(const Coord2D<int>& coord) const;
    const Piece& pieceAt(int x, int y) const;

    Player owner(const Coord2D<int>& coord) const;
    Player owner(int x, int y) const;

    /*! \brief Checks whether move is legal.
     *
     * Method is checking for legality of the passed move, and in the
     * case of legal move it also sets retState to a GameState which
     * will be the state exactly after this move is played.
     * \param retState next state (optional)
     * \param retType move type (optional)
     */
    bool isLegal(Move move, GameState* retState = nullptr, MoveType* retType = nullptr) const;

    /*! \brief Makes the move on the board
     * \return true if the move is done, false otherwise
     */
    bool makeMove(Move move);

    /*! \brief Tests whether current player is in check. */
    bool isCheck() const;

    /*! \brief Tests whether board position is a checkmate. */
    bool isCheckmate() const;

    /*! \brief Tests whether given player has short castling rights. */
    bool hasShortCastlingRights(const Player& player) const;

    /*! \brief Tests whether given player has long castling rights. */
    bool hasLongCastlingRights(const Player& player) const;

    /*! \brief Returns attacked coordinates by the pieces.
     * \return list of attacked coordinates by given piece that is located at given position
     */
    CoordsVector getAttackedCoords(Piece piece, Player player, Coord2D<int> position) const;

    /*! \brief Returns current player */
    Player currentPlayer() const;
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
    GameState m_state;
    mutable Position m_position;
};

#endif // GAME_MODEL_HPP

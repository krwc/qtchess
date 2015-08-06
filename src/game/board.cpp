#include "board.hpp"
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <QDebug>

const GameState Board::InitialGameState = {
    .ShortCastlingRight = {
        { Player::white(), true },
        { Player::black(), true }
    },
    .LongCastlingRight = {
        { Player::white(), true },
        { Player::black(), true }
    },
    .IsCheck = false,
    .WhoIsPlaying = Player::white(),
    .EnPassantPlayer = Player::none()
};

const Coord2D<int> Board::A1 = Coord2D<int>(0, 7);
const Coord2D<int> Board::B1 = Coord2D<int>(1, 7);
const Coord2D<int> Board::C1 = Coord2D<int>(2, 7);
const Coord2D<int> Board::D1 = Coord2D<int>(3, 7);
const Coord2D<int> Board::F1 = Coord2D<int>(5, 7);
const Coord2D<int> Board::G1 = Coord2D<int>(6, 7);
const Coord2D<int> Board::H1 = Coord2D<int>(7, 7);

const Coord2D<int> Board::F8 = Coord2D<int>(5, 0);
const Coord2D<int> Board::G8 = Coord2D<int>(6, 0);
const Coord2D<int> Board::C8 = Coord2D<int>(2, 0);
const Coord2D<int> Board::D8 = Coord2D<int>(3, 0);
const Coord2D<int> Board::A8 = Coord2D<int>(0, 0);
const Coord2D<int> Board::H8 = Coord2D<int>(7, 0);
const Coord2D<int> Board::B8 = Coord2D<int>(1, 0);

Board::Board()
  : mState(Board::InitialGameState)
  , mPosition(Position::defaultPosition())
{
}

QString Board::toFen() const {
    return "";
}

QString Board::fileString(int file) const {
    return QString(file + 'a');
}

QString Board::rankString(int rank) const {
    return QString(rank + '1');
}

QString Board::squareString(Coord2D<int> coord) const {
    return fileString(coord.x) + rankString(7 - coord.y);
}

QString Board::longAlgebraicNotationString(Move move) const {
    return squareString(move.from()) + squareString(move.to());
}

QString Board::algebraicNotationString(Move move) const {
    Piece piece = pieceAt(move.from());
    MoveType moveType;
    GameState gameState;

    if (!isLegal(move, &gameState, &moveType))
        return "invalid move";

    QString check = gameState.IsCheck ? "+" : "";

    switch (moveType) {
    case MOVE_CASTLE_SHORT: return "O-O" + check;
    case MOVE_CASTLE_LONG: return "O-O-O" + check;
    case MOVE_ENPASSANT_CAPTURE:
        return fileString(move.from().x) + "x" + squareString(move.to()) + check;
    case MOVE_PROMOTION:
        return squareString(move.to()) + "=" + piece.symbolString() + check;
    default:
        break;
    }

    // String that helps avoid move ambiguity
    QString uniqueFrom = "";

    // Now have to take care of the notational ambiguity
    if (!piece.isKing() && !piece.isPawn()) {
        CoordsVector attackingPieces;
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                Coord2D<int> current(file, rank);

                if (current == move.from() || pieceAt(current) != piece)
                    continue;
                if (getAttackedCoords(piece, piece.owner(), current).contains(move.to()))
                    attackingPieces.push_back(current);
            }
        }
        int attackersInSameRank = 0;
        int attackersInSameFile = 0;

        for (const auto& item : attackingPieces) {
            if (item.x == move.from().x)
                ++attackersInSameFile;
            if (item.y == move.from().y)
                ++attackersInSameRank;
        }

        if (attackersInSameRank)
            uniqueFrom += fileString(move.from().x);
        if (attackersInSameFile)
            uniqueFrom += rankString(move.from().y);

        // Same diagonal
        if (!attackersInSameRank && !attackersInSameFile && attackingPieces.size())
            uniqueFrom += fileString(move.from().x);
    }
    // Capture
    if (!pieceAt(move.to()).isNone())
        return (piece.isPawn() ? fileString(move.from().x) : piece.symbolString()) +
                uniqueFrom + "x" + squareString(move.to()) + check;

    // Normal move
    return (piece.isPawn() ? "" : piece.symbolString()) + uniqueFrom + squareString(move.to()) + check;
}

const Piece& Board::pieceAt(const Coord2D<int>& coord) const {
    return pieceAt(coord.x, coord.y);
}

const Piece& Board::pieceAt(int x, int y) const {
    assert(isLegalCoord(x, y));
    return mPosition.pieceAt(x, y);
}

Player Board::owner(const Coord2D<int>& coord) const {
    return owner(coord.x, coord.y);
}

Player Board::owner(int x, int y) const {
    assert(isLegalCoord(x, y));
    return mPosition.pieceAt(x, y).owner();
}

bool Board::makeMove(Move move) {
    GameState nextState;
    MoveType type = MOVE_NONSPECIAL;
    if (!isLegal(move, &nextState, &type))
        return false;
    movePieces(move, type);
    mState = nextState;
    // Made the move
    return false;
}

bool Board::isCheck() const {
    return countChecksFor(Player::white()) > 0 ||
           countChecksFor(Player::black()) > 0;
}

bool Board::isCheckmate() const {
    // TODO: stub
    return false;
}

bool Board::hasShortCastlingRights(const Player& player) const {
    return mState.ShortCastlingRight.at(player);
}

bool Board::hasLongCastlingRights(const Player& player) const {
    return mState.LongCastlingRight.at(player);
}

void Board::movePieces(Move move, MoveType type) {
    int sx = move.From.x, sy = move.From.y;
    int dx = move.To.x, dy = move.To.y;

    if (type == MOVE_CASTLE_SHORT) {
        mPosition.setPieceAt(dx-1, dy, mPosition.pieceAt(dx+1, dy));
        mPosition.setPieceAt(dx+1, dy, Piece());
    } else if (type == MOVE_CASTLE_LONG) {
        mPosition.setPieceAt(dx+1, dy, mPosition.pieceAt(dx-2, dy));
        mPosition.setPieceAt(dx-2, dy, Piece());
    } else if (type == MOVE_ENPASSANT_CAPTURE) {
        int y = mState.EnPassantCoords.y + (currentPlayer().isWhite() ? +1 : -1);
        int x = mState.EnPassantCoords.x;
        mPosition.setPieceAt(x, y, Piece());
    }
    
    mPosition.setPieceAt(dx, dy, mPosition.pieceAt(sx, sy));
    mPosition.setPieceAt(sx, sy, Piece());

    if (type == MOVE_PROMOTION)
        mPosition.setPieceAt(dx, dy, Piece(move.PromotionPiece, currentPlayer()));
}

bool Board::canCastle(MoveType castleType) const {
    static std::map<Player, CoordsVector> shortKingWalkSquares = {
        { Player::white(), { F1, G1 } },
        { Player::black(), { F8, G8 } }
    };
    static std::map<Player, CoordsVector> longKingWalkSquares = {
        { Player::white(), { D1, C1 } },
        { Player::black(), { D8, C8 } }
    };

    if (isCheck())
        return false;

    int numKingWalkUnderAttack = 0;
    Player current = currentPlayer();
    Player next = current.opponent();

    if (castleType == MOVE_CASTLE_SHORT && hasShortCastlingRights(current)) {
        for (const auto& step : shortKingWalkSquares[current])
            numKingWalkUnderAttack += countAttacksFor(step, next);
    } else if (castleType == MOVE_CASTLE_LONG && hasLongCastlingRights(current)) {
        for (const auto& step : longKingWalkSquares[current])
            numKingWalkUnderAttack += countAttacksFor(step, next);
    } else
        return false;

    return numKingWalkUnderAttack == 0;
}

bool Board::isInCheckAfterTheMove(Player victim, Move move, MoveType type) const {
    Position backup = mPosition;
    // Yes, I call a non-const method, but after this I restore saved
    // state.
    const_cast<Board*>(this)->movePieces(move, type);
    int numChecks = countChecksFor(victim);
    // Restore board position
    mPosition = backup;
    return numChecks > 0;
}

bool Board::isLegal(Move move, GameState* retState, MoveType* retType) const {
    GameState NextState = mState;

    // Cannot take your own piece
    if (owner(move.To) == currentPlayer())
        return false;
    // Cannot move your opponent piece
    if (owner(move.From) != currentPlayer())
        return false;

    bool legal = false;
    bool moveIsCastle = false;
    MoveType type = MOVE_NONSPECIAL;
    Coord2D<int> enPassantCoords;

    switch (pieceAt(move.From).type()) {
    case Piece::Pawn:   legal = isLegalPawnMove(move, type, enPassantCoords); break;
    case Piece::Knight: legal = isLegalKnightMove(move); break;
    case Piece::Bishop: legal = isLegalBishopMove(move); break;
    case Piece::Rook:   legal = isLegalRookMove(move); break;
    case Piece::Queen:  legal = isLegalQueenMove(move); break;
    case Piece::King:   legal = isLegalKingMove(move, moveIsCastle, type); break;
    default: break;
    }

    if (!legal || isInCheckAfterTheMove(currentPlayer(), move, type))
        return false;
    if (moveIsCastle && !canCastle(type))
        return false;

    // King movement takes away castling rights
    if (pieceAt(move.From).isKing()) {
        NextState.ShortCastlingRight[currentPlayer()] = false;
        NextState.LongCastlingRight[currentPlayer()] = false;
    } else if (pieceAt(move.From).isRook()) {
        // Rook movement too
        if (move.From == H1) NextState.ShortCastlingRight[Player::white()] = false;
        else if (move.From == A1) NextState.LongCastlingRight[Player::white()] = false;
        else if (move.From == H8) NextState.ShortCastlingRight[Player::black()] = false;
        else if (move.From == A8) NextState.LongCastlingRight[Player::black()] = false;
    }
    NextState.WhoIsPlaying = currentPlayer().opponent();
    NextState.IsCheck = isInCheckAfterTheMove(currentPlayer().opponent(), move, type);

    if (retState)
        *retState = NextState;
    if (retType)
        *retType = type;
    return true;
}

bool Board::isLegalPawnMove(Move move, MoveType& Type,
                                Coord2D<int>& EnPassantCoords) const {
    // No backward movement
    if (move.From.y - move.To.y < 0 && currentPlayer().isWhite())
        return false;
    if (move.From.y - move.To.y > 0 && currentPlayer().isBlack())
        return false;

    int Distance = std::abs(move.To.y - move.From.y);
    if (Distance == 2) {
        // Not really a good rank to start such crazy things
        if ((currentPlayer().isWhite() && move.From.y != 6) ||
            (currentPlayer().isBlack() && move.From.y != 1))
            return false;

        if (!isLegalRookMove(move))
            return false;

        // Did we generate EnPassant?
        Coord2D<int> Neighbour[] = { { move.To.x-1, move.To.y },
                                     { move.To.x+1, move.To.y } };
        for (int i = 0; i < 2; i++) {
            if (!isLegalCoord(Neighbour[i]))
                continue;
            if (pieceAt(Neighbour[i]).isPawn() && owner(Neighbour[i]) != currentPlayer()) {
                EnPassantCoords.x = move.To.x;
                EnPassantCoords.y = move.To.y +
                                    (currentPlayer().isWhite() ? +1 : -1);
                // Yes, we did
                Type = MOVE_ENPASSANT_GENERATE;
            }
        }
        return true;
    } else if (Distance == 1) {
        CoordsVector Attacks = getPawnAttack(move.From.x, move.From.y,
                                             currentPlayer());
        bool Promotion = (currentPlayer().isWhite() && move.To.y == 0) ||
                         (currentPlayer().isBlack() && move.To.y == 7);

        // Capture
        if (std::find(Attacks.begin(), Attacks.end(), move.To) != Attacks.end()) {
            // Normal capture
            if (!pieceAt(move.To).isNone() && owner(move.To) != currentPlayer()) {
                if (Promotion)
                    Type = MOVE_PROMOTION;
                return true;
            }
            // En-passant
            else if (mState.EnPassantPlayer == currentPlayer() &&
                     mState.EnPassantCoords == move.To) {
                Type = MOVE_ENPASSANT_CAPTURE;
                return true;
            }
        } else if (pieceAt(move.To).isNone() && isLegalRookMove(move)) {
            if (Promotion)
                Type = MOVE_PROMOTION;
            return true;
        }
        return false;
    }
    return false;
}

bool Board::isLegalKnightMove(Move move) const {
    return getKnightAttack(move.From.x, move.From.y).contains(move.to());
}

bool Board::isLegalBishopMove(Move move) const {
    return getBishopAttack(move.From.x, move.From.y).contains(move.to());
}

bool Board::isLegalRookMove(Move move) const {
    return getRookAttack(move.From.x, move.From.y).contains(move.to());
}

bool Board::isLegalQueenMove(Move move) const {
    return isLegalBishopMove(move) || isLegalRookMove(move);
}

bool Board::isLegalKingMove(Move move, bool& moveIsCastle, MoveType& side) const {
    // Normal king movement
    moveIsCastle = false;
    if (getKingAttack(move.From.x, move.From.y).contains(move.to()))
        return true;

    // Castling move
    bool legal = false;
    if (move.To == G1)
        side = MOVE_CASTLE_SHORT, legal = hasShortCastlingRights(Player::white());
    else if (move.To == C1 && pieceAt(B1).isNone())
        side = MOVE_CASTLE_LONG, legal = hasLongCastlingRights(Player::white());
    else if (move.To == G8)
        side = MOVE_CASTLE_SHORT, legal = hasShortCastlingRights(Player::black());
    else if (move.To == C8 && pieceAt(B8).isNone())
        side = MOVE_CASTLE_LONG, legal = hasShortCastlingRights(Player::black());

    // It is not enough to check if this is the right castle,
    // we need to see there is no blocking pieces on the way.
    if (legal && isLegalRookMove(move))
        return moveIsCastle = true;
    return false;
}

bool Board::isLegalCoord(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool Board::isLegalCoord(Coord2D<int> Coord) const {
    return isLegalCoord(Coord.x, Coord.y);
}

int Board::countAttacksFor(Coord2D<int> coord, Player attacker) const {
    int numAttacks = 0;
    for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
        if (pieceAt(i,j).isNone() || owner(i,j) != attacker)
            continue;
        if (getAttackedCoords(pieceAt(i,j), attacker.opponent(), Coord2D<int>(i,j)).contains(coord))
            ++numAttacks;
    }}
    return numAttacks;
}

int Board::countChecksFor(Player player) const {
    for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
        if (owner(x, y) == player && pieceAt(x, y).isKing())
            return countAttacksFor(Coord2D<int>(x, y), player.opponent());

    assert(!"Amazingly we've lost the King");
    return 0;
}

CoordsVector Board::getAttackedCoords(Piece piece, Player owner, Coord2D<int> position) const {
    int x = position.x;
    int y = position.y;

    switch (piece.type()) {
    case Piece::Pawn:   return getPawnAttack(x, y, owner);
    case Piece::Knight: return getKnightAttack(x, y);
    case Piece::Bishop: return getBishopAttack(x, y);
    case Piece::Rook:   return getRookAttack(x, y);
    case Piece::Queen:  return getQueenAttack(x, y);
    case Piece::King:   return getKingAttack(x, y);
    default:
        return { };
    }
}

Player Board::currentPlayer() const {
    return mState.WhoIsPlaying;
}

CoordsVector Board::getPawnAttack(int x, int y, Player owner) const {
    CoordsVector AttackedCoords;
    int NextY = owner.isWhite() ? y - 1 : y + 1;
    if (isLegalCoord(x+1, NextY)) AttackedCoords.push_back({x+1, NextY});
    if (isLegalCoord(x-1, NextY)) AttackedCoords.push_back({x-1, NextY});
    return AttackedCoords;
}

CoordsVector Board::getBishopAttack(int x, int y) const {
    CoordsVector LegalMoves;

    for (int p = x+1, q = y+1; isLegalCoord(p,q); p++, q++) {
        LegalMoves.push_back({p, q});
        if (pieceAt(p,q).type() != Piece::None) break;
    }

    for (int p = x-1, q = y-1; isLegalCoord(p,q); p--, q--) {
        LegalMoves.push_back({p, q});
        if (pieceAt(p,q).type() != Piece::None) break;
    }

    for (int p = x+1, q = y-1; isLegalCoord(p,q); p++, q--) {
        LegalMoves.push_back({p, q});
        if (pieceAt(p,q).type() != Piece::None) break;
    }

    for (int p = x-1, q = y+1; isLegalCoord(p,q); p--, q++) {
        LegalMoves.push_back({p, q});
        if (pieceAt(p,q).type() != Piece::None) break;
    }

    return LegalMoves;
}

CoordsVector Board::getKnightAttack(int x, int y) const {
    static CoordsVector Jumps = {
        { 1,  2 }, { -1,  2 }, {  2, 1 }, {  2, -1 },
        { 1, -2 }, { -1, -2 }, { -2, 1 }, { -2, -1 }
    };
    CoordsVector AttackedCoords;
    for (Coord2D<int>& Coord : Jumps)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.push_back({Coord.x+x, Coord.y+y});
    return AttackedCoords;
}

CoordsVector Board::getRookAttack(int x, int y) const {
    CoordsVector LegalMoves;

    for (int i = x+1; isLegalCoord(i,y); i++) {
        LegalMoves.push_back({i,y});
        if (pieceAt(i,y).type() != Piece::None) break;
    }

    for (int i = x-1; isLegalCoord(i,y); i--) {
        LegalMoves.push_back({i,y});
        if (pieceAt(i,y).type() != Piece::None) break;
    }

    for (int i = y+1; isLegalCoord(x,i); i++) {
        LegalMoves.push_back({x,i});
        if (pieceAt(x,i).type() != Piece::None) break;
    }

    for (int i = y-1; isLegalCoord(x,i); i--) {
        LegalMoves.push_back({x,i});
        if (pieceAt(x,i).type() != Piece::None) break;
    }
    return LegalMoves;
}

CoordsVector Board::getQueenAttack(int x, int y) const {
    return getRookAttack(x, y) + getBishopAttack(x, y);
}

CoordsVector Board::getKingAttack(int x, int y) const {
    static CoordsVector Moves = {
        { 0,  1 }, {  0, -1 }, {  1, 0 }, { -1, 0 },
        { 1, -1 }, { -1, -1 }, { -1, 1 }, {  1, 1 }
    };
    CoordsVector AttackedCoords;
    for (Coord2D<int>& Coord : Moves)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.push_back({Coord.x+x, Coord.y+y});
    return AttackedCoords;
}

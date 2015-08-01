#include "game-model.hpp"
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <QDebug>

const GameState GameModel::InitialGameState = {
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

#define B(piece) { PIECE_##piece, Player::black() }
#define W(piece) { PIECE_##piece, Player::white() }
#define NONE() { PIECE_NONE, Player::none() }
const Field GameModel::InitialGamePosition[8][8] = {
   { B(ROOK), B(KNIGHT), B(BISHOP), B(QUEEN), B(KING), B(BISHOP), B(KNIGHT), B(ROOK) },
   { B(PAWN), B(PAWN)  , B(PAWN)  , B(PAWN) , B(PAWN), B(PAWN)  , B(PAWN)  , B(PAWN) },
   { NONE() , NONE()   , NONE()   , NONE()  , NONE() , NONE()   , NONE()   , NONE()  },
   { NONE() , NONE()   , NONE()   , NONE()  , NONE() , NONE()   , NONE()   , NONE()  },
   { NONE() , NONE()   , NONE()   , NONE()  , NONE() , NONE()   , NONE()   , NONE()  },
   { NONE() , NONE()   , NONE()   , NONE()  , NONE() , NONE()   , NONE()   , NONE()  },
   { W(PAWN), W(PAWN)  , W(PAWN)  , W(PAWN) , W(PAWN), W(PAWN)  , W(PAWN)  , W(PAWN) },
   { W(ROOK), W(KNIGHT), W(BISHOP), W(QUEEN), W(KING), W(BISHOP), W(KNIGHT), W(ROOK) }
};
#undef W
#undef B

const Coord2D<int> GameModel::A1 = Coord2D<int>(0, 7);
const Coord2D<int> GameModel::B1 = Coord2D<int>(1, 7);
const Coord2D<int> GameModel::C1 = Coord2D<int>(2, 7);
const Coord2D<int> GameModel::D1 = Coord2D<int>(3, 7);
const Coord2D<int> GameModel::F1 = Coord2D<int>(5, 7);
const Coord2D<int> GameModel::G1 = Coord2D<int>(6, 7);
const Coord2D<int> GameModel::H1 = Coord2D<int>(7, 7);

const Coord2D<int> GameModel::F8 = Coord2D<int>(5, 0);
const Coord2D<int> GameModel::G8 = Coord2D<int>(6, 0);
const Coord2D<int> GameModel::C8 = Coord2D<int>(2, 0);
const Coord2D<int> GameModel::D8 = Coord2D<int>(3, 0);
const Coord2D<int> GameModel::A8 = Coord2D<int>(0, 0);
const Coord2D<int> GameModel::H8 = Coord2D<int>(7, 0);
const Coord2D<int> GameModel::B8 = Coord2D<int>(1, 0);

GameModel::GameModel()
  : mState(GameModel::InitialGameState)
{
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        mFields[i][j] = GameModel::InitialGamePosition[i][j];
}

bool GameModel::makeMove(Move move) {
    GameState nextState;
    MoveType type = MOVE_NONSPECIAL;
    if (!isLegal(move, &nextState, &type))
        return false;
    movePieces(move, type);
    mState = nextState;
    // Made the move
    return false;
}

bool GameModel::isCheck() const {
    return countChecksFor(Player::white()) > 0 ||
           countChecksFor(Player::black()) > 0;
}

bool GameModel::isCheckmate() const {
    // TODO: stub
    return false;
}

bool GameModel::hasShortCastlingRights(Player player) const {
    return mState.ShortCastlingRight.at(player);
}

bool GameModel::hasLongCastlingRights(Player player) const {
    return mState.LongCastlingRight.at(player);
}

void GameModel::movePieces(Move move, MoveType type) {
    int sx = move.From.x, sy = move.From.y;
    int dx = move.To.x, dy = move.To.y;

    if (type == MOVE_CASTLE_SHORT) {
        mFields[dy][dx-1] = mFields[dy][dx+1];
        mFields[dy][dx+1] = NONE();
    } else if (type == MOVE_CASTLE_LONG) {
        mFields[dy][dx+1] = mFields[dy][dx-2];
        mFields[dy][dx-2] = NONE();
    } else if (type == MOVE_ENPASSANT_CAPTURE) {
        int y = mState.EnPassantCoords.y + (getCurrentPlayer().isWhite() ? +1 : -1);
        int x = mState.EnPassantCoords.x;
        mFields[y][x] = NONE();
    }

    mFields[dy][dx] = mFields[sy][sx];
    mFields[sy][sx] = NONE();

    if (type == MOVE_PROMOTION)
        mFields[dy][dx].Piece = move.PromotionPiece;
}

bool GameModel::canCastle(MoveType castleType) const {
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
    Player current = getCurrentPlayer();
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

bool GameModel::isInCheckAfterTheMove(Player victim, Move move, MoveType type) const {
    Field backup[8][8];
    // Try the move
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        backup[i][j] = mFields[i][j];
    // Yes, I call a non-const method, but after this I restore saved
    // state.
    const_cast<GameModel*>(this)->movePieces(move, type);
    int numChecks = countChecksFor(victim);
    // Restore board position
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        mFields[i][j] = backup[i][j];
    return numChecks > 0;
}

bool GameModel::isLegal(Move move, GameState* retState, MoveType* retType) const {
    GameState NextState = mState;

    // Cannot take your own piece
    if (owner(move.To) == getCurrentPlayer())
        return false;
    // Cannot move your opponent piece
    if (owner(move.From) != getCurrentPlayer())
        return false;

    bool legal = false;
    bool moveIsCastle = false;
    MoveType type = MOVE_NONSPECIAL;
    Coord2D<int> enPassantCoords;

    switch (piece(move.From)) {
    case PIECE_PAWN:   legal = isLegalPawnMove(move, type, enPassantCoords); break;
    case PIECE_KNIGHT: legal = isLegalKnightMove(move); break;
    case PIECE_BISHOP: legal = isLegalBishopMove(move); break;
    case PIECE_ROOK:   legal = isLegalRookMove(move); break;
    case PIECE_QUEEN:  legal = isLegalQueenMove(move); break;
    case PIECE_KING:   legal = isLegalKingMove(move, moveIsCastle, type); break;
    default: break;
    }

    if (!legal || isInCheckAfterTheMove(getCurrentPlayer(), move, type))
        return false;
    if (moveIsCastle && !canCastle(type))
        return false;

    // King movement takes away castling rights
    if (piece(move.From) == PIECE_KING) {
        NextState.ShortCastlingRight[getCurrentPlayer()] = false;
        NextState.LongCastlingRight[getCurrentPlayer()] = false;
    } else if (piece(move.From) == PIECE_ROOK) {
        // Rook movement too
        if (move.From == H1) NextState.ShortCastlingRight[Player::white()] = false;
        else if (move.From == A1) NextState.LongCastlingRight[Player::white()] = false;
        else if (move.From == H8) NextState.ShortCastlingRight[Player::black()] = false;
        else if (move.From == A8) NextState.LongCastlingRight[Player::black()] = false;
    }
    NextState.WhoIsPlaying = getCurrentPlayer().opponent();
    NextState.IsCheck = isInCheckAfterTheMove(getCurrentPlayer().opponent(), move, type);

    if (retState)
        *retState = NextState;
    if (retType)
        *retType = type;
    return true;
}

bool GameModel::isLegalPawnMove(Move move, MoveType& Type,
                                Coord2D<int>& EnPassantCoords) const {
    // No backward movement
    if (move.From.y - move.To.y < 0 && mState.WhoIsPlaying.isWhite())
        return false;
    if (move.From.y - move.To.y > 0 && mState.WhoIsPlaying.isBlack())
        return false;

    int Distance = std::abs(move.To.y - move.From.y);
    if (Distance == 2) {
        // Not really a good rank to start such crazy things
        if ((mState.WhoIsPlaying.isWhite() && move.From.y != 6) ||
            (mState.WhoIsPlaying.isBlack() && move.From.y != 1))
            return false;

        if (!isLegalRookMove(move))
            return false;

        // Did we generate EnPassant?
        Coord2D<int> Neighbour[] = { { move.To.x-1, move.To.y },
                                     { move.To.x+1, move.To.y } };
        for (int i = 0; i < 2; i++) {
            if (!isLegalCoord(Neighbour[i]))
                continue;
            if (piece(Neighbour[i]) == PIECE_PAWN &&
                owner(Neighbour[i]) != mState.WhoIsPlaying) {
                EnPassantCoords.x = move.To.x;
                EnPassantCoords.y = move.To.y +
                                    (mState.WhoIsPlaying.isWhite() ? +1 : -1);
                // Yes, we did
                Type = MOVE_ENPASSANT_GENERATE;
            }
        }
        return true;
    } else if (Distance == 1) {
        CoordsVector Attacks = getPawnAttack(move.From.x, move.From.y,
                                             mState.WhoIsPlaying);
        bool Promotion = (mState.WhoIsPlaying.isWhite() && move.To.y == 0) ||
                         (mState.WhoIsPlaying.isBlack() && move.To.y == 7);

        // Capture
        if (std::find(Attacks.begin(), Attacks.end(), move.To) != Attacks.end()) {
            // Normal capture
            if (piece(move.To) != PIECE_NONE && owner(move.To) != mState.WhoIsPlaying) {
                if (Promotion)
                    Type = MOVE_PROMOTION;
                return true;
            }
            // En-passant
            else if (mState.EnPassantPlayer == mState.WhoIsPlaying &&
                     mState.EnPassantCoords == move.To) {
                Type = MOVE_ENPASSANT_CAPTURE;
                return true;
            }
        } else if (piece(move.To) == PIECE_NONE && isLegalRookMove(move)) {
            if (Promotion)
                Type = MOVE_PROMOTION;
            return true;
        }
        return false;
    }
    return false;
}

bool GameModel::isLegalKnightMove(Move move) const {
    return Contains(getKnightAttack(move.From.x, move.From.y), move.To);
}

bool GameModel::isLegalBishopMove(Move move) const {
    return Contains(getBishopAttack(move.From.x, move.From.y), move.To);
}

bool GameModel::isLegalRookMove(Move move) const {
    return Contains(getRookAttack(move.From.x, move.From.y), move.To);
}

bool GameModel::isLegalQueenMove(Move move) const {
    return isLegalBishopMove(move) || isLegalRookMove(move);
}

bool GameModel::isLegalKingMove(Move move, bool& moveIsCastle, MoveType& side) const {
    // Normal king movement
    moveIsCastle = false;
    if (Contains(getKingAttack(move.From.x, move.From.y), move.To))
        return true;

    // Castling move
    bool legal = false;
    if (move.To == G1)
        side = MOVE_CASTLE_SHORT, legal = hasShortCastlingRights(Player::white());
    else if (move.To == C1 && piece(B1) == PIECE_NONE)
        side = MOVE_CASTLE_LONG, legal = hasLongCastlingRights(Player::white());
    else if (move.To == G8)
        side = MOVE_CASTLE_SHORT, legal = hasShortCastlingRights(Player::black());
    else if (move.To == C8 && piece(B8) == PIECE_NONE)
        side = MOVE_CASTLE_LONG, legal = hasShortCastlingRights(Player::black());

    // It is not enough to check if this is the right castle,
    // we need to see there is no blocking pieces on the way.
    if (legal && isLegalRookMove(move))
        return moveIsCastle = true;
    return false;
}

bool GameModel::isLegalCoord(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool GameModel::isLegalCoord(Coord2D<int> Coord) const {
    return isLegalCoord(Coord.x, Coord.y);
}

int GameModel::countAttacksFor(Coord2D<int> coord, Player attacker) const {
    int numAttacks = 0;
    for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
        if (piece(i,j) == PIECE_NONE || owner(i,j) != attacker)
            continue;
        if (Contains(getAttackedCoords(piece(i,j), attacker.opponent(), Coord2D<int>(i,j)), coord))
            ++numAttacks;
    }}
    return numAttacks;
}

int GameModel::countChecksFor(Player player) const {
    for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
        if (owner(x, y) == player && piece(x, y) == PIECE_KING)
            return countAttacksFor(Coord2D<int>(x, y), player.opponent());

    assert(!"Amazingly we've lost the King");
    return 0;
}

CoordsVector GameModel::getAttackedCoords(Piece piece, Player owner, Coord2D<int> position) const {
    int x = position.x;
    int y = position.y;

    switch (piece) {
    case PIECE_PAWN:   return getPawnAttack(x, y, owner);
    case PIECE_KNIGHT: return getKnightAttack(x, y);
    case PIECE_BISHOP: return getBishopAttack(x, y);
    case PIECE_ROOK:   return getRookAttack(x, y);
    case PIECE_QUEEN:  return getQueenAttack(x, y);
    case PIECE_KING:   return getKingAttack(x, y);
    default:
        return { };
    }
}

CoordsVector GameModel::getPawnAttack(int x, int y, Player owner) const {
    CoordsVector AttackedCoords;
    int NextY = owner.isWhite() ? y - 1 : y + 1;
    if (isLegalCoord(x+1, NextY)) AttackedCoords.emplace_back(x+1, NextY);
    if (isLegalCoord(x-1, NextY)) AttackedCoords.emplace_back(x-1, NextY);
    return AttackedCoords;
}

CoordsVector GameModel::getBishopAttack(int x, int y) const {
    CoordsVector LegalMoves;

    for (int p = x+1, q = y+1; isLegalCoord(p,q); p++, q++) {
        LegalMoves.emplace_back(p, q);
        if (piece(p,q) != PIECE_NONE) break;
    }

    for (int p = x-1, q = y-1; isLegalCoord(p,q); p--, q--) {
        LegalMoves.emplace_back(p, q);
        if (piece(p,q) != PIECE_NONE) break;
    }

    for (int p = x+1, q = y-1; isLegalCoord(p,q); p++, q--) {
        LegalMoves.emplace_back(p, q);
        if (piece(p,q) != PIECE_NONE) break;
    }

    for (int p = x-1, q = y+1; isLegalCoord(p,q); p--, q++) {
        LegalMoves.emplace_back(p, q);
        if (piece(p,q) != PIECE_NONE) break;
    }

    return LegalMoves;
}

CoordsVector GameModel::getKnightAttack(int x, int y) const {
    static CoordsVector Jumps = {
        { 1,  2 }, { -1,  2 }, {  2, 1 }, {  2, -1 },
        { 1, -2 }, { -1, -2 }, { -2, 1 }, { -2, -1 }
    };
    CoordsVector AttackedCoords;
    for (Coord2D<int>& Coord : Jumps)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.emplace_back(Coord.x+x, Coord.y+y);
    return AttackedCoords;
}

CoordsVector GameModel::getRookAttack(int x, int y) const {
    CoordsVector LegalMoves;

    for (int i = x+1; isLegalCoord(i,y); i++) {
        LegalMoves.emplace_back(i,y);
        if (piece(i,y) != PIECE_NONE) break;
    }

    for (int i = x-1; isLegalCoord(i,y); i--) {
        LegalMoves.emplace_back(i,y);
        if (piece(i,y) != PIECE_NONE) break;
    }

    for (int i = y+1; isLegalCoord(x,i); i++) {
        LegalMoves.emplace_back(x,i);
        if (piece(x,i) != PIECE_NONE) break;
    }

    for (int i = y-1; isLegalCoord(x,i); i--) {
        LegalMoves.emplace_back(x, i);
        if (piece(x,i) != PIECE_NONE) break;
    }
    return LegalMoves;
}

CoordsVector GameModel::getQueenAttack(int x, int y) const {
    CoordsVector Attacks = getRookAttack(x, y);
    CoordsVector BishopAttacks = getBishopAttack(x, y);
    Attacks.insert(Attacks.end(), BishopAttacks.begin(), BishopAttacks.end());
    return Attacks;
}

CoordsVector GameModel::getKingAttack(int x, int y) const {
    static CoordsVector Moves = {
        { 0,  1 }, {  0, -1 }, {  1, 0 }, { -1, 0 },
        { 1, -1 }, { -1, -1 }, { -1, 1 }, {  1, 1 }
    };
    CoordsVector AttackedCoords;
    for (Coord2D<int>& Coord : Moves)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.emplace_back(Coord.x+x, Coord.y+y);
    return AttackedCoords;
}

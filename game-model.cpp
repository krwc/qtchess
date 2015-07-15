#include "game-model.hpp"
#include <cstdlib>
#include <cstdio>
#include <algorithm>

const GameState GameModel::InitialGameState = {
    .ShortCastlingRight = { true, true },
    .LongCastlingRight = { true, true },
    .CheckedPlayer = PLAYER_NONE,
    .WhoIsPlaying = PLAYER_WHITE
};

#define B(piece) { PIECE_##piece, PLAYER_BLACK }
#define W(piece) { PIECE_##piece, PLAYER_WHITE }
#define NONE() { PIECE_NONE, PLAYER_NONE }
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
    GameState NextState;
    MoveType MoveType = MOVE_NONSPECIAL;
    if (!isLegal(move, &NextState, &MoveType))
        return false;
    movePieces(move, MoveType);
    mState = NextState;
    // Made the move
    return true;
}

bool GameModel::isCheck() const {
    return countChecksFor(mState.WhoIsPlaying) > 0;
}

bool GameModel::isCheckmate() const {
    // TODO: stub
    return false;
}

void GameModel::movePieces(Move move, MoveType Type) {
    // Get the rooks rollin'
    if (Type == MOVE_CASTLE_SHORT) {
        mFields[move.To.y][move.To.x-1] = mFields[move.To.y][move.To.x+1];
        mFields[move.To.y][move.To.x+1] = NONE();
    } else if (Type == MOVE_CASTLE_LONG) {
        mFields[move.To.y][move.To.x+1] = mFields[move.To.y][move.To.x-2];
        mFields[move.To.y][move.To.x-2] = NONE();
    } else if (Type == MOVE_ENPASSANT_CAPTURE) {
        int y = mState.EnPassantCoords.y + (mState.WhoIsPlaying == PLAYER_WHITE ? +1 : -1);
        int x = mState.EnPassantCoords.x;

        mFields[y][x] = NONE();
    }

    mFields[move.To.y][move.To.x]     = mFields[move.From.y][move.From.x];
    mFields[move.From.y][move.From.x] = NONE();

    if (Type == MOVE_PROMOTION)
        mFields[move.To.y][move.To.x].Piece = move.PromotionPiece;
}

bool GameModel::isCheckAfterTheMove(Move move, MoveType Type) {
    Field Backup[8][8];
    Player Curr = mState.WhoIsPlaying;
    Player Next = Player(!mState.WhoIsPlaying);

    if (Type == MOVE_CASTLE_SHORT) {
        if (Curr == PLAYER_WHITE)
            return countAttacksFor(F1, Next) + countAttacksFor(G1, Next);
        else
            return countAttacksFor(F8, Next) + countAttacksFor(G8, Next);
    } else if (Type == MOVE_CASTLE_LONG) {
        if (Curr == PLAYER_WHITE)
            return countAttacksFor(D1, Next) + countAttacksFor(C1, Next);
        else
            return countAttacksFor(D8, Next) + countAttacksFor(C8, Next);
    }

    // Try the move
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        Backup[i][j] = mFields[i][j];
    movePieces(move, Type);
    int NumChecks = countChecksFor(Curr);
    // Restore board state
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        mFields[i][j] = Backup[i][j];
    return NumChecks;
}

bool GameModel::isLegal(Move move, GameState* RetState, MoveType* Type) {
    GameState NextState = mState;
    const Field& Dst = mFields[move.To.y][move.To.x];
    const Field& Src = mFields[move.From.y][move.From.x];
    // Cannot take your own piece
    if (Dst.Owner == mState.WhoIsPlaying)
        return false;
    // Cannot move your opponent piece
    if (Src.Owner != mState.WhoIsPlaying)
        return false;

    bool Legal;
    bool MoveIsCastle = false;
    MoveType NonStandardMove = MOVE_NONSPECIAL;
    Coord2D<int> EnPassantCoords;
    switch (Src.Piece) {
        case PIECE_PAWN:
            Legal = isLegalPawnMove(move, NonStandardMove, EnPassantCoords); break;
        case PIECE_KNIGHT:
            Legal = isLegalKnightMove(move); break;
        case PIECE_BISHOP:
            Legal = isLegalBishopMove(move); break;
        case PIECE_ROOK:
            Legal = isLegalRookMove(move); break;
        case PIECE_KING:
            Legal = isLegalKingMove(move, MoveIsCastle, NonStandardMove); break;
        case PIECE_QUEEN:
            Legal = isLegalQueenMove(move); break;
        default:
            Legal = false;
    }
    if (!Legal || isCheckAfterTheMove(move, NonStandardMove))
        return false;
    if (isCheck() && MoveIsCastle)
        return false;

    // King move takes away castling rights
    if (Src.Piece == PIECE_KING) {
        NextState.ShortCastlingRight[mState.WhoIsPlaying] = false;
        NextState.LongCastlingRight[mState.WhoIsPlaying]  = false;
    }
    // Rook move too.
    else if (Src.Piece == PIECE_ROOK) {
        if (move.From == H1) NextState.ShortCastlingRight[PLAYER_WHITE] = false;
        else if (move.From == A1) NextState.LongCastlingRight[PLAYER_WHITE] = false;
        else if (move.From == H8) NextState.ShortCastlingRight[PLAYER_BLACK] = false;
        else if (move.From == A8) NextState.LongCastlingRight[PLAYER_BLACK] = false;
    } else if (Src.Piece == PIECE_PAWN) {
        if (NonStandardMove == MOVE_ENPASSANT_GENERATE) {
            NextState.EnPassantPlayer = Player(!NextState.WhoIsPlaying);
            NextState.EnPassantCoords = EnPassantCoords;
        } else
            NextState.EnPassantPlayer = PLAYER_NONE;
    }

    if (move.To == A1) NextState.LongCastlingRight[PLAYER_WHITE] = false;
    else if (move.To == A8) NextState.LongCastlingRight[PLAYER_BLACK] = false;
    else if (move.To == H1) NextState.ShortCastlingRight[PLAYER_WHITE] = false;
    else if (move.To == H8) NextState.ShortCastlingRight[PLAYER_BLACK] = false;

    NextState.WhoIsPlaying = Player(!NextState.WhoIsPlaying);

    if (RetState)
        *RetState = NextState;
    if (Type)
        *Type = NonStandardMove;
    return true;
}

bool GameModel::isLegalPawnMove(Move move, MoveType& Type,
                                 Coord2D<int>& EnPassantCoords) const {
    // No backward movement
    if (move.From.y - move.To.y < 0 && mState.WhoIsPlaying == PLAYER_WHITE)
        return false;
    if (move.From.y - move.To.y > 0 && mState.WhoIsPlaying == PLAYER_BLACK)
        return false;

    int Distance = std::abs(move.To.y - move.From.y);
    if (Distance == 2) {
        // Not really a good rank to start such crazy things
        if ((mState.WhoIsPlaying == PLAYER_WHITE && move.From.y != 6) ||
            (mState.WhoIsPlaying == PLAYER_BLACK && move.From.y != 1))
            return false;

        if (!isLegalRookMove(move))
            return false;

        // Did we generate EnPassant?
        Coord2D<int> Neighbour[] = { { move.To.x-1, move.To.y },
                                     { move.To.x+1, move.To.y } };
        for (int i = 0; i < 2; i++) {
            if (!isLegalCoord(Neighbour[i]))
                continue;
            if (getField(Neighbour[i]).Piece == PIECE_PAWN &&
                getField(Neighbour[i]).Owner != mState.WhoIsPlaying) {
                EnPassantCoords.x = move.To.x;
                EnPassantCoords.y = move.To.y +
                                    (mState.WhoIsPlaying == PLAYER_WHITE ? +1 : -1);
                // We just generated en-passant opportunity
                Type = MOVE_ENPASSANT_GENERATE;
            }
        }
        return true;
    } else if (Distance == 1) {
        std::vector<Coord2D<int>> Attacks = getPawnAttack(move.From.x, move.From.y,
                                                          mState.WhoIsPlaying);
        bool Promotion = (mState.WhoIsPlaying == PLAYER_WHITE && move.To.y == 0) ||
                         (mState.WhoIsPlaying == PLAYER_BLACK && move.To.y == 7);

        // Capture
        if (std::find(Attacks.begin(), Attacks.end(), move.To) != Attacks.end()) {
            // Normal capture
            if (getField(move.To).Piece != PIECE_NONE &&
                getField(move.To).Owner != mState.WhoIsPlaying) {
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
        } else if (getField(move.To).Piece == PIECE_NONE && isLegalRookMove(move)) {
            if (Promotion)
                Type = MOVE_PROMOTION;
            return true;
        }
        return false;
    }
    return false;
}

bool GameModel::isLegalKnightMove(Move move) const {
    // It is unbeliveable that the most tricky piece has that
    // easy moving rules.
    for (Coord2D<int> Jump : getKnightAttack(move.From.x, move.From.y))
        if (Jump == move.To)
            return true;
    return false;
}

bool GameModel::isLegalBishopMove(Move move) const {
    // Legal moves from current position of the bishop.
    std::vector<Coord2D<int>> LegalMoves;
    // Iterate till no other move can be generated
    int D = 1;
    size_t NumMoves = 0;
    do {
        int x = move.From.x;
        int y = move.From.y;
        NumMoves = LegalMoves.size();

        if (isLegalCoord(x+D,y+D)) LegalMoves.emplace_back(x+D,y+D);
        if (isLegalCoord(x-D,y-D)) LegalMoves.emplace_back(x-D,y-D);
        if (isLegalCoord(x+D,y-D)) LegalMoves.emplace_back(x+D,y-D);
        if (isLegalCoord(x-D,y+D)) LegalMoves.emplace_back(x-D,y+D);
        ++D;
    } while (LegalMoves.size() - NumMoves);
    // Check if there is a coordinate we want to go to on
    // our list of generated legal moves if not, move is illegal
    if (std::find(LegalMoves.begin(), LegalMoves.end(), move.To) == LegalMoves.end())
        return false;

    // Now check if bishop moving path is clear - that is, there are
    // no blocking pieces.
    int dx = (move.From.x - move.To.x) / std::abs(move.From.x - move.To.x);
    int dy = (move.From.y - move.To.y) / std::abs(move.From.y - move.To.y);
    int x = move.To.x + dx;
    int y = move.To.y + dy;

    while (x != move.From.x && y != move.From.y) {
        // Blocker detected
        if (getField(x, y).Piece != PIECE_NONE)
            return false;
        x += dx;
        y += dy;
    }

    return true;
}

bool GameModel::isLegalRookMove(Move move) const {
    // Clearly not in the same file / rank
    if (move.From.x != move.To.x && move.From.y != move.To.y)
        return false;

    // Check for blockers
    int dx = (move.From.x - move.To.x);
    int dy = (move.From.y - move.To.y);
    dx = dx < 0 ? -1 : dx > 0 ? +1 : 0;
    dy = dy < 0 ? -1 : dy > 0 ? +1 : 0;
    int x = move.To.x + dx;
    int y = move.To.y + dy;

    while (x != move.From.x || y != move.From.y) {
        if (getField(x, y).Piece != PIECE_NONE)
            return false;
        x += dx;
        y += dy;
    }
    return true;
}

bool GameModel::isLegalQueenMove(Move move) const {
    return isLegalBishopMove(move) || isLegalRookMove(move);
}

bool GameModel::isLegalKingMove(Move move, bool& MoveIsCastle, MoveType& Side) const {
    // Normal king move
    MoveIsCastle = false;
    if (std::abs(move.From.x - move.To.x) <= 1 &&
        std::abs(move.From.y - move.To.y) <= 1)
        return true;

    // Castling move
    bool Legal = false;
    if (move.To == G1)
        Side = MOVE_CASTLE_SHORT, Legal = mState.ShortCastlingRight[PLAYER_WHITE];
    else if (move.To == C1 && getField(B1).Piece == PIECE_NONE)
        Side = MOVE_CASTLE_LONG, Legal = mState.LongCastlingRight[PLAYER_WHITE];
    else if (move.To == G8)
        Side = MOVE_CASTLE_SHORT, Legal = mState.ShortCastlingRight[PLAYER_BLACK];
    else if (move.To == C8 && getField(B8).Piece == PIECE_NONE)
        Side = MOVE_CASTLE_LONG, Legal = mState.LongCastlingRight[PLAYER_BLACK];
    else return false;

    // It is not enough to check if this is the right castle,
    // we need to see there is no blocking pieces on the way.
    if (Legal && isLegalRookMove(move))
        return MoveIsCastle = true;
    return false;
}

bool GameModel::isLegalCoord(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool GameModel::isLegalCoord(Coord2D<int> Coord) const {
    return isLegalCoord(Coord.x, Coord.y);
}

int GameModel::countAttacksFor(Coord2D<int> Coord, Player Attacker) const {
    int NumAttacks = 0;
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            const Field& Field = getField(i, j);
            if (Field.Piece == PIECE_NONE || Field.Owner != Attacker)
                continue;

            // FIXME: Isn't that looking a bit crazy?
            if (Field.Piece == PIECE_PAWN && Contains(getPawnAttack(i, j, Attacker), Coord))
                ++NumAttacks;
            else if (Field.Piece == PIECE_KNIGHT && Contains(getKnightAttack(i, j), Coord))
                ++NumAttacks;
            else if (Field.Piece == PIECE_BISHOP && Contains(getBishopAttack(i, j), Coord))
                ++NumAttacks;
            else if (Field.Piece == PIECE_ROOK && Contains(getRookAttack(i, j), Coord))
                ++NumAttacks;
            else if (Field.Piece == PIECE_QUEEN && Contains(getQueenAttack(i, j), Coord))
                ++NumAttacks;
            else if (Field.Piece == PIECE_KING && Contains(getKingAttack(i, j), Coord))
                ++NumAttacks;
        }
    }
    return NumAttacks;
}

int GameModel::countChecksFor(Player player) const {
    for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
        if (getField(x, y).Owner == player && getField(x, y).Piece == PIECE_KING)
            return countAttacksFor(Coord2D<int>(x, y), Player(!player));
    assert(!"Amazingly we've lost the King");
    return 0;
}

std::vector<Coord2D<int>> GameModel::getPawnAttack(int x, int y, Player Owner) const {
    std::vector<Coord2D<int>> AttackedCoords;
    if (Owner == PLAYER_WHITE) {
        if (isLegalCoord(x+1, y-1)) AttackedCoords.emplace_back(x+1, y-1);
        if (isLegalCoord(x-1, y-1)) AttackedCoords.emplace_back(x-1, y-1);
    } else {
        if (isLegalCoord(x+1, y+1)) AttackedCoords.emplace_back(x+1, y+1);
        if (isLegalCoord(x-1, y+1)) AttackedCoords.emplace_back(x-1, y+1);
    }
    return AttackedCoords;
}

std::vector<Coord2D<int>> GameModel::getBishopAttack(int x, int y) const {
    std::vector<Coord2D<int>> AttackedCoords;
    Coord2D<int> Src(x,y);
    for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
        if ((x == i && y == j))
            continue;
        if (isLegalBishopMove(Move(Src, Coord2D<int>(i,j))))
            AttackedCoords.emplace_back(i, j);
    }}
    return AttackedCoords;
}

std::vector<Coord2D<int>> GameModel::getKnightAttack(int x, int y) const {
    static std::vector<Coord2D<int>> Jumps = {
        { 1,  2 }, { -1,  2 }, {  2, 1 }, {  2, -1 },
        { 1, -2 }, { -1, -2 }, { -2, 1 }, { -2, -1 }
    };
    std::vector<Coord2D<int>> AttackedCoords;
    for (Coord2D<int>& Coord : Jumps)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.emplace_back(Coord.x+x, Coord.y+y);
    return AttackedCoords;
}

std::vector<Coord2D<int>> GameModel::getRookAttack(int x, int y) const {
    std::vector<Coord2D<int>> AttackedCoords;
    Coord2D<int> Src(x,y);
    for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
        if ((x == i && y == j))
            continue;
        if (isLegalRookMove(Move(Src, Coord2D<int>(i,j))))
            AttackedCoords.emplace_back(i, j);
    }}
    return AttackedCoords;
}

std::vector<Coord2D<int>> GameModel::getQueenAttack(int x, int y) const {
    std::vector<Coord2D<int>> Attacks = getRookAttack(x, y);
    std::vector<Coord2D<int>> BishopAttacks = getBishopAttack(x, y);
    Attacks.insert(Attacks.end(), BishopAttacks.begin(), BishopAttacks.end());
    return Attacks;
}

std::vector<Coord2D<int>> GameModel::getKingAttack(int x, int y) const {
    static std::vector<Coord2D<int>> Moves = {
        { 0,  1 }, {  0, -1 }, {  1, 0 }, { -1, 0 },
        { 1, -1 }, { -1, -1 }, { -1, 1 }, {  1, 1 }
    };
    std::vector<Coord2D<int>> AttackedCoords;
    for (Coord2D<int>& Coord : Moves)
        if (isLegalCoord(Coord.x+x, Coord.y+y))
            AttackedCoords.emplace_back(Coord.x+x, Coord.y+y);
    return AttackedCoords;
}

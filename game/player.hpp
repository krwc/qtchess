#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
public:
    Player();

    bool isWhite() const { return mKind == PLAYER_WHITE; }
    bool isBlack() const { return mKind == PLAYER_BLACK; }
    bool isNone() const { return mKind == PLAYER_NONE; }
    Player opponent() const;

    bool operator== (const Player& player) const {
        return mKind == player.mKind;
    }

    bool operator!= (const Player& player) const {
        return mKind != player.mKind;
    }

    bool operator< (const Player& player) const {
        return mKind < player.mKind;
    }

    static Player white();
    static Player black();
    static Player none();
private:
    enum PlayerKind { PLAYER_WHITE = 0, PLAYER_BLACK, PLAYER_NONE };
    Player(PlayerKind);

    PlayerKind mKind;
};

#endif // PLAYER_HPP

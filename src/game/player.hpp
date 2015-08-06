#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
public:
    Player();

    bool isWhite() const { return mKind == White; }
    bool isBlack() const { return mKind == Black; }
    bool isNone() const { return mKind == None; }
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
    enum PlayerKind { White = 0, Black, None };
    Player(PlayerKind);

    PlayerKind mKind;
};

#endif // PLAYER_HPP

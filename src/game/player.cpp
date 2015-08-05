#include "player.hpp"

Player::Player()
  : Player(PLAYER_NONE)
{ }

Player::Player(PlayerKind kind)
  : mKind(kind)
{ }

Player Player::white() {
    return Player(PLAYER_WHITE);
}

Player Player::black() {
    return Player(PLAYER_BLACK);
}

Player Player::none() {
    return Player(PLAYER_NONE);
}

Player Player::opponent() const {
    if (isWhite())
        return Player::black();
    else if (isBlack())
        return Player::white();
    else
        return Player::none();
}

#include "player.hpp"

Player::Player()
  : Player(None)
{ }

Player::Player(PlayerKind kind)
  : mKind(kind)
{ }

Player Player::white() {
    return Player(White);
}

Player Player::black() {
    return Player(Black);
}

Player Player::none() {
    return Player(None);
}

Player Player::opponent() const {
    if (isWhite())
        return Player::black();
    else if (isBlack())
        return Player::white();
    else
        return Player::none();
}

#include <Player.h>

Player::Player(GameWindow * game_window) :
    Entity(game_window)
{

}

void Player::update(double delta){
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
}
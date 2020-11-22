#include <Player.h>

void Player::update(double delta){
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
}
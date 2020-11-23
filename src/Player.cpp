#include <Player.h>

void Player::update(double delta){
    move_dir = glm::vec3(move_dir[0],0,move_dir[2]);
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
}
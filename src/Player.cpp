#include <Player.h>

void Player::update(double delta){
    move_dir = glm::vec3(move_dir[0],0,move_dir[2]);
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
}

void Player::resolve_collisions() {
    for (const auto e : collides_with_list) {
        if (e->collision_box) {
            // TODO: check for and resolve collision with e->collision_box.
        }
    }
}
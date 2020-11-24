#include <Player.h>

Player::Player() {
    collision_box = CollisionBox(
        glm::vec3(0.0f, head_height/2.0f, 0.0f), // center
        glm::vec3(girth, head_height/2.0f, girth)  // extents
    );
}

void Player::update(double delta){
    move_dir = glm::vec3(move_dir[0],0,move_dir[2]);
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
}

void Player::resolve_collisions() {
    if (!walking) return;
    for (const auto e : collides_with_list) {
        if (e->get_collision_box()) {
            
            // either null_opt or collision normal
            auto n_opt = collision_box.value().collision_with(get_pos(), e->get_pos(), e->get_collision_box().value());

            if (n_opt){ // if there was a collision
                auto & n = *n_opt; // collision normal, pointing in direction we want to avoid
                float dot = glm::dot(move_dir, n);
                if (dot > 0) {
                    // Kill the component of move_dir that points into collision plane, then renormalize
                    move_dir = move_dir - dot * n;
                }
            }
        }
    }
}
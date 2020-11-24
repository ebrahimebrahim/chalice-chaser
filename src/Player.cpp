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
            
            // either null_opt or CollisionData
            auto collision_data_opt = collision_box.value().collision_with(get_pos(), e->get_pos(), e->get_collision_box().value());

            if (collision_data_opt){ // if there was a collision
                auto & n = collision_data_opt->normal; // collision normal, pointing in direction we want to avoid
                auto & d = collision_data_opt->distance; // distance to back out if we want to no longer be colliding
                float dot = glm::dot(move_dir, n);
                if (dot > 0) {
                    // Kill the component of move_dir that points into collision plane, then renormalize
                    move_dir = move_dir - dot * n;
                }

                pos -= 0.5f*d*n;

            }
        }
    }
}
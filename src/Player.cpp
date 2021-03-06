#include <Player.h>
#include <Prize.h>
#include <Portal.h>

Player::Player(Timer* t) : Entity(), timer{t} {
    collision_box = CollisionBox(
        glm::vec3(0.0f, head_height/2.0f, 0.0f), // center
        glm::vec3(girth, head_height/2.0f, girth)  // extents
    );
}

void Player::update(double delta){
    move_dir = glm::vec3(move_dir[0],0,move_dir[2]);
    if (walking)
        pos += float(walk_speed * delta) * move_dir;
    if (timer->timed_out())
        lost = true;
}


void Player::resolve_collisions() {
    Entity::resolve_collisions(); // prune collisions marked for deletion
    if (!walking) return;
    for (const auto e : collides_with_list) {
        if (e->get_collision_box()) {
            
            // either null_opt or CollisionData
            auto collision_data_opt = collision_box.value().collision_with(get_pos(), e->get_pos(), e->get_collision_box().value());

            if (collision_data_opt){ // if there was a collision

                if (!has_prize && dynamic_cast<const Prize*>(e)) { // if it's with prize, we grab the prize
                    has_prize = true;
                    e->mark_for_deletion();
                    timer->start();
                }
                else if (has_prize && dynamic_cast<const Portal*>(e)) { // if it's with the portal and we already have prize, then win
                    won = true;
                }
                else { // if collision is with anything else

                    auto & n = collision_data_opt->normal; // collision normal, pointing in direction we want to avoid
                    auto & d = collision_data_opt->distance; // distance to back out if we want to no longer be colliding
                    float dot = glm::dot(move_dir, n);
                    if (dot > 0) {
                        // Kill the component of move_dir that points into collision plane, then renormalize
                        move_dir = move_dir - dot * n;
                    }

                    // Make progress towards getting out of the collision situation
                    pos -= 0.5f*d*n;
                    // (without this, it's easy for player to get locked out of both axes of motion by ending up in several very small collisions
                    // e.g. think about what happens as you rub against a wall which is made of many collision cubes)
                    // (if we do `pos -= d*n` then you get bouncing)
                
                }

            }
        }
    }
}
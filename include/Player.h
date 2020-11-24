#ifndef PLAYER_H
#define PLAYER_H

#include <Entity.h>
#include <glm/glm.hpp>


class Player : public Entity {

    bool walking = false; /** player movement state, intended to be set on each frame by some outside input handler */
    
    /** Current player movement direction vector, intended to be set on each frame by some outside input handler.
     *  It is typically a unit vector, but `resolve_collisions` can make it smaller.
     */
    glm::vec3 move_dir{0.0f,0.0f,0.0f};

public:
    
    Player();

    const float head_height = 1.5f; /** The position of a player is the position of the player's feet. */
    const float walk_speed = 10.0f; /** world coord units per second */
    const float girth = 0.4f;

    void update(double delta) override;
    void draw(glm::vec3) const override {} // we don't draw anything for player
    void resolve_collisions() override;

    /** `dir` should be a unit vector, otherwise it will mess with player's speed.
     *  (If you want to mess with speed see member variable `walk_speed`.) */
    void set_walking(glm::vec3 dir) {walking=true; move_dir=dir;}
    void set_not_walking() {walking=false;}
};

#endif // PLAYER_H
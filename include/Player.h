#ifndef PLAYER_H
#define PLAYER_H

#include <Entity.h>
#include <glm/glm.hpp>


class Player : public Entity {
    bool walking = false; /** player movement state, intended to be set on each frame by some outside input handler */
    glm::vec3 move_dir{0.0f,0.0f,0.0f}; /** current player movement direction vector, intended to be set on each frame by some outside input handler */
public:
    glm::vec3 pos{0.0f,0.0f,0.0f}; /** world coords of player's feet */
    const float head_height = 1.5f;
    const float walk_speed = 6.0f; /** world coord units per second */

    void update(double delta) override;
    void draw() const override {} // we don't draw anything for player

    /** `dir` should be a unit vector, otherwise it will mess with player's speed.
     *  (If you want to mess with speed see member variable `walk_speed`.) */
    void set_walking(glm::vec3 dir) {walking=true; move_dir=dir;}
    void set_not_walking() {walking=false;}
};

#endif // PLAYER_H
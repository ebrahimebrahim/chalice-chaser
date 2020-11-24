#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
    static int next_id;
    int id;

public:
    Entity() {id = next_id++;}
    virtual ~Entity() = default;
    int get_id() const {return id;}
    virtual void update(double delta) = 0; /** delta is the time step of the update in seconds */
    virtual void draw(glm::vec3 player_position) const = 0;
    virtual void resolve_collisions() {}
};

#endif // ENTITY_H
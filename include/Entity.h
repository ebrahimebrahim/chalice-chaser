#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <CollisionBox.h>

class Entity {
    static int next_id;
    int id;

protected:
    glm::vec3 pos{0.0f,0.0f,0.0f};
    std::vector<const Entity *> collides_with_list; /** what other entities this entity collides with */

public:
    Entity() {id = next_id++;}
    virtual ~Entity() = default;
    int get_id() const {return id;}
    virtual void update(double delta) = 0; /** delta is the time step of the update in seconds */
    virtual void draw(glm::vec3 player_position) const = 0;
    virtual void resolve_collisions() {} /** what to do when there is a collision */

    void collides_with(Entity * e) {collides_with_list.push_back(e);}

    auto get_pos() const {return pos;}
    virtual void set_pos(const glm::vec3 new_pos) {pos=new_pos;}

    std::optional<CollisionBox> collision_box;
};

#endif // ENTITY_H
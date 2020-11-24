#include <Entity.h>

int Entity::next_id = 0;

Entity::Entity(const Entity & src) :
    pos(src.pos), collides_with_list(src.collides_with_list), collision_box(src.collision_box)
{
    id = next_id++;
}


Entity::Entity(Entity && src) :
    pos(src.pos), collides_with_list(std::move(src.collides_with_list)), collision_box(std::move(src.collision_box))
{
    id = src.id;
    src.id = -1;
}


Entity & Entity::operator=(const Entity & src) {
    if (this == &src) return *this;
    id = next_id++;

    pos = src.pos;
    collides_with_list = src.collides_with_list;
    collision_box = src.collision_box;
    return *this;
}


Entity & Entity::operator=(Entity && src) {
    if (this == &src) return *this;
    id = src.id;
    src.id = -1;    

    pos = src.pos;
    collides_with_list = std::move(src.collides_with_list);
    collision_box = std::move(src.collision_box);
    return *this;
}


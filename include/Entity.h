#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <vector>
#include <experimental/optional>
#include <CollisionBox.h>

class Entity {
    static int next_id;
    int id;

protected:
    glm::vec3 pos{0.0f,0.0f,0.0f};
    std::vector<Entity *> collides_with_list; /** what other entities this entity collides with */
    std::experimental::optional<CollisionBox> collision_box;

public:
    Entity() {id = next_id++;}
    Entity(const Entity &);
    Entity(Entity &&);
    Entity & operator=(const Entity &);
    Entity & operator=(Entity &&);
    virtual ~Entity() {}
    
    int get_id() const {return id;}
    virtual void update(double delta) = 0; /**< delta is the time step of the update in seconds */
    virtual void draw(glm::vec3 player_position) const = 0;

    /** What to do when there is a collision.
     * Make sure to prune collides_with_list of entities that are marked for deletion.
     * You can do this by just calling `Entity::resolve_collisions` when you override it,
     * or you can handle the pruning yourself inside your collision resolution loop.
     * */
    virtual void resolve_collisions(); 

    void collides_with(Entity * e) {collides_with_list.push_back(e);}

    auto get_pos() const {return pos;}
    virtual void set_pos(const glm::vec3 new_pos) {pos=new_pos;}
    const auto & get_collision_box() const {return collision_box;}


    /**If a system in the game wants to delete an entity, then it should mark the entity for deletion
     * using this method. The entity will not immediately be deleted. Instead, it will be allowed to live
     * for another game update, so that other systems get a chance to see that their handles to the entity
     * are about to become invalid.
    */
    void mark_for_deletion() {marked_for_deletion=true;}
    bool marked_for_deletion{false};
    bool ready_to_be_deleted{false};
};

#endif // ENTITY_H
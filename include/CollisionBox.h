#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include <glm/glm.hpp>
#include <optional>

/** An axis-aligned cuboid the represents a collision region in object-local coords. */
class CollisionBox {
    
    glm::vec3 center; /** In object coords. So the world location of the center of the collision box is the object's `pos` plus `center`*/
    glm::vec3 extents; /** Half-extents along each axis */

public:

    CollisionBox(glm::vec3 center, glm::vec3 extents) :
        center(center), extents(extents)
        {}
    
    /**Determines if there is a collison and if so returns a vector normal to the collision plane.
     * The normal vector points towards the `other_box`, in the direction that ought to be avoided
     * by the owner of the present `CollisionBox`.
     * `my_pos` is the world position of the object that owns this `CollisionBox`.
     * `other_pos` is the world position of the object that owns `other_box`.
     * 
     * The normal vector is guaranteed to be a unit vector.
    */
    std::optional<glm::vec3> collision_with(glm::vec3 my_pos, glm::vec3 other_pos, CollisionBox other_box);
    
    

    
};

#endif // COLLISION_BOX_H
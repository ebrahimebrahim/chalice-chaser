#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include <glm/glm.hpp>

class CollisionBox {
    
    glm::vec3 center;
    glm::vec3 extents;

public:

    CollisionBox(glm::vec3 center, glm::vec3 extents) :
        center(center), extents(extents)
        {}
    
    

    
};

#endif // COLLISION_BOX_H
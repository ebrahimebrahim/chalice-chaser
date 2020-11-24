#include <CollisionBox.h>

const glm::vec3 X_DIR{1.0f, 0.0f, 0.0f};
const glm::vec3 Y_DIR{0.0f, 1.0f, 0.0f};
const glm::vec3 Z_DIR{0.0f, 0.0f, 1.0f};
const glm::vec3 UNIT_VECTOR[3] = {X_DIR, Y_DIR, Z_DIR};

std::optional<CollisionData> CollisionBox::collision_with(glm::vec3 my_pos, glm::vec3 other_pos, CollisionBox other_box) {
    
    // displacement vector from my center to other center, in the world coords
    glm::vec3 d = other_pos + other_box.center - (my_pos + center);
    
    glm::vec3 overlaps = extents + other_box.extents - glm::abs(d);

    if (overlaps[0] > 0 && overlaps[1] > 0 && overlaps[2] > 0){
        int dir_of_smallest_overlap;
        if (overlaps[0] < overlaps[1] && overlaps[0] < overlaps[2]) { // if x direction has smallest overlap
            dir_of_smallest_overlap = 0;
        }
        else if (overlaps[1] < overlaps[2]) { // if y direction has smallest overlap
            dir_of_smallest_overlap = 1;
        } 
        else { // if z direction has smallest overlap
            dir_of_smallest_overlap = 2;
        }

        return CollisionData{
            glm::sign(d[dir_of_smallest_overlap]) * UNIT_VECTOR[dir_of_smallest_overlap], // collision normal
            overlaps[dir_of_smallest_overlap] // collision distance
        };
    }
 
    return std::nullopt; // no collision
}
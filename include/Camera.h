#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct Camera {

    /** Position of camera in world coordinates */
    glm::vec3 pos;

    /** Unit vector representing the direction the camera points. */
    glm::vec3 dir;

    /** Unit vector pointing to the right of the camera, orthogonal to `dir`. */
    glm::vec3 right;

    /** Unit vector pointing in the direction of the world's up.  */
    glm::vec3 world_up;

    /** radians per screen pixel */
    const float mouse_sensitivity = 0.0015f; 

    glm::mat4 get_view_matrix();

};

#endif // CAMERA_H
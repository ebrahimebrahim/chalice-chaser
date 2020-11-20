#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {

    /** Position of camera in world coordinates */
    glm::vec3 pos;

    /** Unit vector representing the direction the camera points. */
    glm::vec3 dir;

    /** Unit vector pointing to the right of the camera, orthogonal to `dir`. */
    glm::vec3 right;

    /** Unit vector pointing in the direction of the world's up.  */
    glm::vec3 world_up;

public:

    /** Initialize camera at given position, star
     *  looking at given target, and with its "up" being set as given.
    */
    Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

    auto get_pos() const {return pos;}
    auto get_dir() const {return dir;}
    auto get_right() const {return right;}

    void set_pos(glm::vec3 new_pos) {pos = new_pos;}
    void look_at(glm::vec3 target);

    /** radians per screen pixel */
    const float mouse_sensitivity = 0.0015f; 

    glm::mat4 get_view_matrix();

    void update(glm::vec3 new_pos, glm::vec2 mouse_delta);

};

#endif // CAMERA_H
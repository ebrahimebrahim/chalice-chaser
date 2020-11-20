#include <Camera.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/common.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) :
    world_up(glm::normalize(up))
{
    set_pos(pos);
    look_at(target);
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(pos,pos+dir,world_up);
}

void Camera::look_at(glm::vec3 target) {
    dir = glm::normalize(target - pos);
    right = glm::normalize(glm::cross(dir,world_up));
}

void Camera::update(glm::vec3 new_pos, glm::vec2 mouse_delta) {
    
    // set location
    set_pos(new_pos);
    
    // set yaw
    dir = glm::rotate(dir,-mouse_sensitivity*mouse_delta[0],world_up);
    
    // set pitch
    glm::vec3 new_dir = glm::rotate(dir,-mouse_sensitivity*mouse_delta[1],right);
    if (glm::abs(glm::dot(new_dir,world_up)) < 0.95f) // don't allow pitch beyond zenith/nadir
        dir = new_dir;

    // recalculate right
    right = glm::normalize(glm::cross(dir,world_up));
    
}
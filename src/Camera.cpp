#include <Camera.h>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(pos,pos+dir,world_up);
}
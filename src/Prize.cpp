#include <Prize.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Prize::Prize(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.0f, 0.0f, 0.0f}, // center
        glm::vec3{0.8f, 1.0f, 0.8f}  // extents
    );
}

GraphicsData Prize::create_graphics_data() {
    GraphicsData d;
    d.vertices = {
        0.0f, 0.0f, 0.0f , // center of fan
        -0.3f, -0.3f, -0.3f ,
        0.3f, -0.3f, -0.3f ,
        0.3f, -0.3f, 0.3f ,
        -0.3f, -0.3f, 0.3f ,
    };
    d.indices = { 0,1,2,3,4 };
    d.shader_choice = ShaderChoice::SHADER_DEFAULT;
    d.draw_mode = GL_TRIANGLE_FAN;
    return d;
    
}


void Prize::update(double delta) {
    rotate((TAU/3.0f) * delta, glm::vec3(0.0f,1.0f,0.0f));
}
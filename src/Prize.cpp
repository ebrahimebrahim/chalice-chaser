#include <Prize.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


std::optional<GraphicsData> Prize::graphics_data; // start as std::null_opt

GraphicsData generate_prize_graphics_data() {
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


Prize::Prize(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.0f, 0.0f, 0.0f}, // center
        glm::vec3{0.8f, 1.0f, 0.8f}  // extents
    );
}

GraphicsData Prize::create_graphics_data() {
    return get_graphics_data();
}

GraphicsData Prize::get_graphics_data() {
    if (!graphics_data)
        graphics_data = generate_prize_graphics_data();
    return *graphics_data;
}

void Prize::update(double delta) {
    rotate((TAU/3.0f) * delta, glm::vec3(0.0f,1.0f,0.0f));
}
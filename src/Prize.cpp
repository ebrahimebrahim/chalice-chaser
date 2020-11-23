#include <Prize.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Prize::Prize(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
}

GraphicsData Prize::create_graphics_data() {
    GraphicsData d;
    d.num_vertices = 5;
    GLfloat prize_vertices[d.num_vertices][3] = {
        { 0.0f, 0.0f, 0.0f }, // center of fan
        { -0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, 0.3f },
        { -0.3f, -0.3f, 0.3f },
    };
    d.vertices = (GLfloat*)prize_vertices;
    d.num_indices = 5;
    GLuint prize_indices[d.num_indices] = { 0,1,2,3,4 };
    d.indices = prize_indices;
    d.shader_choice = ShaderChoice::SHADER_DEFAULT;
    d.draw_mode = GL_TRIANGLE_FAN;
    return d;
    
}


void Prize::update(double delta) {
    rotate((TAU/3.0f) * delta, glm::vec3(0.0f,1.0f,0.0f));
}
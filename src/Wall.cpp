#include <Wall.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Wall::Wall(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
}

GraphicsData Wall::create_graphics_data() {
    GraphicsData d;
    d.num_vertices = 8;
    const float wall_height = 2.0;
    GLfloat vertices[d.num_vertices][3] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, wall_height, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, wall_height, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, wall_height, 1.0f },
        { 1.0f, 0.0f, 1.0f },
        { 1.0f, wall_height, 1.0f },
    };
    d.vertices = (GLfloat*)vertices;
    d.num_indices = 10;
    GLuint indices[d.num_indices] = { 0,1,2,3,6,7,4,5,0,1 };
    d.indices = indices;
    d.shader_choice = SHADER_DEFAULT;
    d.draw_mode = GL_TRIANGLE_STRIP;
    return d;
    
}


void Wall::update(double delta) {
    
}
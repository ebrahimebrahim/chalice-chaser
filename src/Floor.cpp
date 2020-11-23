#include <Floor.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Floor::Floor(GameWindow * game_window, float xdim, float zdim) :
    GraphicalEntity(game_window),
    xdim(xdim), zdim(zdim)
{
    add_self_to_game_window();
}

GraphicsData Floor::create_graphics_data() {
    GraphicsData d;
    d.num_vertices = 4;
    GLfloat vertices[d.num_vertices][3] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, zdim },
        { xdim, 0.0f, 0.0f },
        { xdim, 0.0f, zdim },
    };
    d.vertices = (GLfloat*)vertices;
    d.num_indices = 6;
    GLuint indices[d.num_indices] = { 0,1,3, 0,3,2 };
    d.indices = indices;
    d.shader_choice = SHADER_DEFAULT;
    d.draw_mode = GL_TRIANGLES;
    return d;
    
}


void Floor::update(double delta) {
    
}
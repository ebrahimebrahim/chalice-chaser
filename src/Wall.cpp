#include <Wall.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


const float Wall::wall_height = 2.0f;

Wall::Wall(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.5f, wall_height/2.0f, 0.5f}, // center
        glm::vec3{0.5f, wall_height/2.0f, 0.5f}  // extents
    );
}

GraphicsData Wall::create_graphics_data() {
    GraphicsData d;
    d.num_vertices = 8;
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
#include <Wall.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


const float Wall::wall_height = 2.0f;

Wall::Wall(GameWindow * game_window) : 
    GraphicalEntity(game_window)
{
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.5f, wall_height/2.0f, 0.5f}, // center
        glm::vec3{0.5f, wall_height/2.0f, 0.5f}  // extents
    );
}

GraphicsData Wall::create_graphics_data() {
    GraphicsData d;
    d.vertices ={
        0.0f, 0.0f, 0.0f ,          0.0f, 0.0f,
        0.0f, wall_height, 0.0f ,   0.0f, 1.0f,
        1.0f, 0.0f, 0.0f ,          0.25f, 0.0f,
        1.0f, wall_height, 0.0f ,   0.25f, 1.0f,
    };
    d.indices = {0,1,2,3};
    d.shader_choice = SHADER_TEXTURE;
    d.draw_mode = GL_TRIANGLE_STRIP;
    d.texture = std::make_shared<StbImage>("images/brick.png");
    return d;
    
}


void Wall::update(double delta) {
}


void Wall::rotate_about_block_center(float angle){
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5,0.0,0.5));
}
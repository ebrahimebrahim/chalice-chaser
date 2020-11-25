#include <Portal.h>
#include <Wall.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


const float Portal::height = Wall::wall_height;

Portal::Portal(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.5f, height/2.0f, 0.1f}, // center
        glm::vec3{0.5f, height/2.0f, 0.4f}  // extents
    );
}

GraphicsData Portal::create_graphics_data() {
    GraphicsData d;
    d.vertices ={
        0.0f, 0.0f, 0.1f ,
        0.0f, height, 0.1f ,
        1.0f, 0.0f, 0.1f ,
        1.0f, height, 0.1f ,
    };
    d.indices = { 0, 1, 3, 0, 3, 2 };
    d.shader_choice = SHADER_PORTAL;
    d.draw_mode = GL_TRIANGLE_STRIP;
    return d;
    
}


void Portal::update(double delta) {
    phase += 0.1;
    if (phase > TAU) phase -= TAU;
}

void Portal::draw(glm::vec3 player_position) const {
    game_window->set_uniform(SHADER_PORTAL, "player_pos", player_position);
    game_window->set_uniform(SHADER_PORTAL, "phase", phase);
    game_window->set_object_model_matrix(get_id(),get_model_matrix());
    game_window->draw(get_id());
}
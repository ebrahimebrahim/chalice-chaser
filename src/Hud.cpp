#include <Hud.h>
#include <GameWindow.h>
#include <Player.h>
#include <glm/gtx/transform.hpp>

Hud::Hud(GameWindow * window, const Player * player) : Entity(), game_window(window), player(player) {
    GraphicsData d;
    d.num_vertices = 5;
    GLfloat prize_vertices[d.num_vertices][3] = { // TODO get this from the same source as Prize.cpp, instead of duplicating vertex data.
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
    d.shader_choice = ShaderChoice::SHADER_HUD;
    d.draw_mode = GL_TRIANGLE_FAN;
    game_window->add_object(get_id(), d);
    glm::mat4 hud_model = glm::translate(glm::vec3(-0.95f,0.95f,0.0f)) * glm::scale(glm::vec3(0.1f,0.3f,1.0f));

    game_window->set_object_model_matrix(get_id(), hud_model);
}

Hud::~Hud() {
    game_window->del_object(get_id());
}

void Hud::draw(glm::vec3 player_position) const {
    if (player->has_prize)
        game_window->draw(get_id());
}

void Hud::update(double delta) {

}


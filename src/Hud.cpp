#include <Hud.h>
#include <GameWindow.h>
#include <Player.h>
#include <glm/gtx/transform.hpp>

void Hud::update() {
    if (!player_has_prize && player->has_prize){
        player_has_prize = true;
        prize_hud = std::make_unique<PrizeHud>(game_window);
    }
}

void Hud::draw() const {
    if (prize_hud) {
        prize_hud->draw(player->get_pos());
    }
}


PrizeHud::PrizeHud(GameWindow * window) : Entity(), game_window(window) {
    GraphicsData d;
    d.vertices = { // TODO get this from the same source as Prize.cpp, instead of duplicating vertex data.
        0.0f, 0.0f, 0.0f , // center of fan
        -0.3f, -0.3f, -0.3f ,
        0.3f, -0.3f, -0.3f ,
        0.3f, -0.3f, 0.3f ,
        -0.3f, -0.3f, 0.3f ,
    };
    d.indices = { 0,1,2,3,4 };
    d.shader_choice = ShaderChoice::SHADER_HUD;
    d.draw_mode = GL_TRIANGLE_FAN;
    game_window->add_object(get_id(), d);
    glm::mat4 hud_model = glm::translate(glm::vec3(-0.95f,0.95f,0.0f)) * glm::scale(glm::vec3(0.1f,0.3f,1.0f));

    game_window->set_object_model_matrix(get_id(), hud_model);
}

PrizeHud::~PrizeHud() {
    game_window->del_object(get_id());
}

void PrizeHud::draw(glm::vec3 player_position) const {
    game_window->draw(get_id());
}

void PrizeHud::update(double delta) {

}


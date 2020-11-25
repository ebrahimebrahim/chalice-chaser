#include <Hud.h>
#include <GameWindow.h>
#include <Player.h>
#include <glm/gtx/transform.hpp>
#include <Prize.h>

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
    GraphicsData d = Prize::get_graphics_data();
    d.shader_choice = ShaderChoice::SHADER_HUD;

    const float aspect_ratio = float(game_window->get_height())/float(game_window->get_width()); 
    glm::mat4 hud_model = glm::translate(glm::vec3(-0.85f,0.85f,0.0f)) * glm::scale(glm::vec3(0.15f * aspect_ratio, 0.15f, 1.0f));

    game_window->add_object(get_id(), d);
    game_window->set_object_model_matrix(get_id(), hud_model);
}

PrizeHud::~PrizeHud() {
    game_window->del_object(get_id());
}

void PrizeHud::draw(glm::vec3 player_position) const {
    glDisable(GL_DEPTH_TEST);  // Probably this should be done via method provided by GameWindow, but w/e.
    game_window->draw(get_id());
    glEnable(GL_DEPTH_TEST); 
}

void PrizeHud::update(double delta) {

}


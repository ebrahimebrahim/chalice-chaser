#include <Hud.h>
#include <GameWindow.h>
#include <Player.h>
#include <glm/gtx/transform.hpp>
#include <Prize.h>

void Hud::update() {
    if (!player_has_prize && player->has_prize){
        player_has_prize = true;
        prize_hud = std::make_unique<PrizeHud>(game_window);
        timer_hud = std::make_unique<TimerHud>(game_window);
    }
    if (timer_hud) {
        timer_hud->set_fraction(timer->get_fraction_remaining());
        timer_hud->update();
    }
    if (prize_hud)
        prize_hud->update();
}

void Hud::draw() const {
    glDisable(GL_DEPTH_TEST);  // Probably this should be done via method provided by GameWindow, but w/e.
    if (prize_hud) {
        prize_hud->draw(player->get_pos());
    }
    if (timer_hud) {
        timer_hud->draw(player->get_pos());
    }
    if (message_hud)
        message_hud->draw(player->get_pos());
    glEnable(GL_DEPTH_TEST); 
}

void Hud::show_message(const char * texture_filename) {
    message_hud = std::make_unique<MessageHud>(game_window,texture_filename,0.75);
}

// ----------------- PrizeHud ----------------

PrizeHud::PrizeHud(GameWindow * window) : Entity(), game_window(window) {
    GraphicsData d = Prize::get_graphics_data();
    d.shader_choice = ShaderChoice::SHADER_HUD;
    d.object_color = std::experimental::nullopt;

    const float aspect_ratio = float(game_window->get_height())/float(game_window->get_width()); 
    glm::mat4 hud_model = glm::translate(glm::vec3(-1 + 0.15*aspect_ratio,0.85f,0.0f)) * glm::scale(glm::vec3(0.15f * aspect_ratio, 0.15f, 1.0f));

    game_window->add_object(get_id(), d);
    game_window->set_object_model_matrix(get_id(), hud_model);
}

PrizeHud::~PrizeHud() {
    game_window->del_object(get_id());
}

void PrizeHud::draw(glm::vec3 player_position) const {
    game_window->draw(get_id());
}

void PrizeHud::update() {
    
}

// ----------------- TimerHud ----------------

TimerHud::TimerHud(GameWindow * window) : Entity(), game_window(window) {
    GraphicsData d;
    d.vertices = {
        0, 0, 0,
        max_width, 0, 0,
        0, height, 0,
        max_width, height, 0
    };
    d.indices = {
        0, 1, 2,
        2, 1, 3
    } ;
    d.draw_mode = GL_TRIANGLES;
    d.shader_choice = ShaderChoice::SHADER_HUD;
    game_window->add_object(get_id(), d);

    translate = glm::translate(glm::vec3(botleft_pos,0.0f));
}

TimerHud::~TimerHud() {
    game_window->del_object(get_id());
}

void TimerHud::draw(glm::vec3 player_position) const {
    game_window->set_object_model_matrix(get_id(), translate * scale);
    game_window->draw(get_id());
}

void TimerHud::update() {
    scale = glm::scale(glm::vec3(fraction,1.0f,1.0f));
}


// ----------------- MessageHud ----------------

#include <iostream>
MessageHud::MessageHud(GameWindow * game_window, const char * texture_filename, float width) :
    Entity(),
    game_window(game_window)
{
    GraphicsData d;
    d.texture = std::make_shared<StbImage>(texture_filename);
    const float window_aspect_ratio = float(game_window->get_width())/float(game_window->get_height()); 
    const float height = window_aspect_ratio * (width / d.texture->get_width()) * d.texture->get_height();
    d.vertices = {
        -width/2.0f, -height/2.0f, 0.0f,  0.0f, 0.0f,
         width/2.0f, -height/2.0f, 0.0f,  1.0f, 0.0f,
        -width/2.0f,  height/2.0f, 0.0f,  0.0f, 1.0f,
         width/2.0f,  height/2.0f, 0.0f,  1.0f, 1.0f
    };
    d.indices = {
        0, 1, 2,
        2, 1, 3
    } ;
    d.draw_mode = GL_TRIANGLES;
    d.shader_choice = ShaderChoice::SHADER_HUD_TEXTURE;
    game_window->add_object(get_id(), d);
}

MessageHud::~MessageHud() {
    game_window->del_object(get_id());
}

void MessageHud::draw(glm::vec3 player_position) const {
    game_window->draw(get_id());
}
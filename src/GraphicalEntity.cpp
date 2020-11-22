#include <GraphicalEntity.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

GraphicalEntity::GraphicalEntity(GameWindow * game_window) :
    game_window(game_window),
    model_matrix{1.0f},
    model_matrix_without_translation{1.0f}
{
    
}

void GraphicalEntity::add_self_to_game_window() {
    game_window->add_object(get_id(),create_graphics_data());
}

GraphicalEntity::GraphicalEntity(const GraphicalEntity & src) : 
    game_window(src.game_window),
    model_matrix{src.model_matrix},
    model_matrix_without_translation{src.model_matrix_without_translation}
{
    game_window->duplicate_object(src.get_id(), get_id());
}

GraphicalEntity & GraphicalEntity::operator=(const GraphicalEntity & src) {
    if (&src==this) return *this;
    game_window = src.game_window;
    model_matrix = src.model_matrix;
    model_matrix_without_translation = src.model_matrix_without_translation;
    game_window->duplicate_object(src.get_id(), get_id());
    return *this;
}

GraphicalEntity::GraphicalEntity(GraphicalEntity && src) :
    game_window(src.game_window),
    model_matrix{src.model_matrix},
    model_matrix_without_translation{src.model_matrix_without_translation}
{
    game_window->change_object_id(src.get_id(), get_id());
}


GraphicalEntity & GraphicalEntity::operator=(GraphicalEntity && src) {
    if (&src==this) return *this;
    game_window = src.game_window;
    model_matrix = src.model_matrix;
    model_matrix_without_translation = src.model_matrix_without_translation;
    game_window->change_object_id(src.get_id(), get_id());
    return *this;
}

GraphicalEntity::~GraphicalEntity() {
    game_window->del_object(get_id());
}

void GraphicalEntity::draw() const {
    game_window->set_object_model_matrix(get_id(),model_matrix);
    game_window->draw(get_id());
}


void GraphicalEntity::update_model_matrix() {
    model_matrix = glm::translate(pos) * model_matrix_without_translation;
}

void GraphicalEntity::set_pos(const glm::vec3 new_pos) {
    pos = new_pos;
    update_model_matrix();
}

void GraphicalEntity::rotate(float angle, glm::vec3 axis) {
    model_matrix_without_translation = glm::rotate(angle, axis) * model_matrix_without_translation;
    update_model_matrix();
}
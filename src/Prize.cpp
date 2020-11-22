#include <Prize.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const float TAU = 6.2831853f;

Prize::Prize(GameWindow * game_window) 
    : Entity(game_window), model_matrix{1.0f}
{
    GraphicsData d;
    d.num_vertices = 5;
    GLfloat prize_vertices[d.num_vertices][3] = {
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
    d.shader_choice = ShaderChoice::DEFAULT;
    d.draw_mode = GL_TRIANGLE_FAN;
    game_window->add_object(get_id(),d);
}

Prize::Prize(const Prize & src) : Entity(game_window) {
    model_matrix = src.model_matrix;
    rot=src.rot;
    game_window->duplicate_object(src.get_id(), get_id());
}

Prize & Prize::operator=(const Prize & src) {
    if (&src==this) return *this;
    model_matrix = src.model_matrix;
    rot=src.rot;
    game_window->duplicate_object(src.get_id(), get_id());
    return *this;
}

Prize::Prize(Prize && src) : Entity(game_window) {
    model_matrix = src.model_matrix;
    rot = src.rot;
    game_window->change_object_id(src.get_id(), get_id());
}


Prize & Prize::operator=(Prize && src) {
    if (&src==this) return *this;
    model_matrix = src.model_matrix;
    rot = src.rot;
    game_window->change_object_id(src.get_id(), get_id());
    return *this;
}

Prize::~Prize() {
    game_window->del_object(get_id());
}



void Prize::update(double delta) {
    rot += (TAU/3.0f) * delta;
    model_matrix = glm::translate(pos) * glm::rotate(rot, glm::vec3(0.0f,1.0f,0.0f));
}


void Prize::draw() const {
    game_window->set_object_model_matrix(get_id(),model_matrix);
    game_window->draw(get_id());
}
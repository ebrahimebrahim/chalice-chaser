#include<Prize.h>
#include<GameWindow.h>
#include <glm/ext/matrix_transform.hpp>

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
    d.vertex_shader_path = "src/shader.vert";
    d.fragment_shader_path = "src/shader.frag";
    d.draw_mode = GL_TRIANGLE_FAN;
    game_window->add_object(get_id(),d);
}



void Prize::update(double delta) {
    model_matrix = glm::rotate(model_matrix, (TAU/3.0f) * float(delta),glm::vec3(0.0f,1.0f,0.0f));
}


void Prize::draw() {
    game_window->set_object_model_matrix(get_id(),model_matrix);
    game_window->draw(get_id());
}
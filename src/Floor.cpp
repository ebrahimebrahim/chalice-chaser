#include <Floor.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Floor::Floor(GameWindow * game_window, float xdim, float zdim) :
    GraphicalEntity(game_window),
    xdim(xdim), zdim(zdim)
{
    add_self_to_game_window();
}

GraphicsData Floor::create_graphics_data() {
    GraphicsData d;
    d.vertices = {
        0.0f, 0.0f, 0.0f , 0.0, 0.0,
        0.0f, 0.0f, zdim , 0.0, 1.0,
        xdim, 0.0f, 0.0f , 1.0, 0.0,
        xdim, 0.0f, zdim , 1.0, 1.0,
    };
    d.indices = { 0,1,3, 0,3,2 };
    d.shader_choice = SHADER_TEXTURE;
    d.draw_mode = GL_TRIANGLES;
    d.texture = std::make_shared<StbImage>("images/lose.png");
    return d;
    
}


void Floor::update(double delta) {
    
}
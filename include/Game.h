#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>


/**
    The game loop
*/

class Game {

    /** Create the window and the opengl context and load resources. Unload later with with `Game::uninit` */
    void init();

    /** Unload what was loaded via Game::init */
    void uninit();

    /** Handle user input in the game loop */
    void handle_input();

    /** Update game by one tick/frame/whatever */
    void update();

    /** Render game state into framebuffer and swap buffers */
    void render();
    
    /** Get current game fps */
    double getFPS() {return fps;}

    int window_width = 800;
    int window_height = 600;

public:
    /** Run the game, returning return code on exit. 0 means no errors. */
    int run();

    /** Fixed update step size in milliseconds, for deterministic updates to game state */
    const std::chrono::nanoseconds time_per_update{16'666'666};

    /** Caps framerate */
    const std::chrono::nanoseconds min_time_per_frame{16'666'666};


    double fps;

    /** Handle to the game window */
    GLFWwindow * window;

};

#endif // GAME_H
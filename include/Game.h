#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

    /** Update game state by one time_per_update */
    void update();

    /** Render game state into framebuffer and swap buffers */
    void render();

    /** Handle a keypress that was polled via GLFW. */
    void handle_key(int key, int scancode, int action, int mods);
    
    /** Get current game fps */
    double getFPS() {return 1.0/last_frame_time;}

    int window_width = 800;
    int window_height = 600;

public:
    /** Run the game, returning return code on exit. 0 means no errors. */
    int run();

    /** Fixed update step size in seconds, for deterministic updates to game state */
    const double time_per_update{0.016666667};

    /** Time spent on the last frame of rendering, in seconds. */
    double last_frame_time{};

    bool quit = false;

    /** Handle to the game window */
    GLFWwindow * window;

};

#endif // GAME_H
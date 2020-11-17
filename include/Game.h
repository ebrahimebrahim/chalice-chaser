#ifndef GAME_H
#define GAME_H

#include <chrono>


/**
    The game loop
*/

class Game {

    /** Create the window and the opengl context and load resources. */
    void init();

    /** Handle user input in the game loop */
    void handle_input();

    /** Update game by one tick/frame/whatever */
    void update();

    /** Render game state into framebuffer and swap buffers */
    void render();
    
    /** Get current game fps */
    double getFPS() {return fps;}

public:
    /** Run the game, returning return code on exit. 0 means no errors. */
    int run();

    /** Fixed update step size in milliseconds, for deterministic updates to game state */
    const std::chrono::milliseconds time_per_update{100};

    /** Caps framerate */
    const std::chrono::milliseconds min_time_per_frame{500};

    /** Current game fps */
    double fps;
};

#endif // GAME_H
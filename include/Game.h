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
    

public:
    /** Run the game, returning return code on exit. 0 means no errors. */
    int run();
};
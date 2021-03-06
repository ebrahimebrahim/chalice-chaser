#ifndef GAME_H
#define GAME_H

#include <GameWindow.h>
#include <memory>
#include <vector>
#include <Entity.h>
#include <Camera.h>
#include <Player.h>
#include <Wall.h>
#include <level_gen.h>
#include <Hud.h>
#include <Timer.h>

/** FRONT is +z, BACK is -z, LEFT is +x, RIGHT is -x*/
enum class WallOrientation {FRONT, BACK, LEFT, RIGHT};

/**
    The game loop
*/

class Game {

    /** Make window, creating an opengl context */
    void init_window();

    /** Make level, player, entities, hud, and camera */
    void create_game_objects();

    /** Clear out game objects so that a later call to `create_game_objects` essentially restarts the game */
    void reset_game();

    /** Handle user input in the game loop */
    void handle_input();

    /** Render game state into framebuffer and swap buffers */
    void render();
    
    /** Time spent on the last frame of rendering, in seconds. */
    double last_frame_time{};

    bool paused{false};

    /** Mouse movement vector on the last frame of rendering */
    glm::vec2 last_frame_mouse_delta{0.0f,0.0f};

    std::unique_ptr<GameWindow> window;
    std::vector<std::unique_ptr<Entity>> entities;
    std::unique_ptr<Camera> camera;
    Player * player{}; /**< handle to player, which is a regular entity that should be in the std::vector of entities */
    LevelGen::Tilemap level;
    std::unique_ptr<Hud> hud;
    Timer * timer; /**< handle to game timer, which is a regular entity that should be in `entities` */

    /**Make a wall using the prototype, place it at the specified position, make player collide with it, and add it to the list of entities.
     * Note that `Game::player` *must* be initialized before this is used.
     * The purpose of the prototype is to copy it so that opengl geometry data can be shared among wall instances.
     * The created wall is rotated according to the given `WallOrientation`.
     * FRONT is +z, BACK is -z, LEFT is +x, RIGHT is -x
    */
    void place_wall(const glm::vec3 & pos, Wall * prototype_wall, WallOrientation orientation);

public:
    /** Run the game, returning return code on exit. 0 means no errors. */
    int run();

    /** Fixed update step size in seconds, for deterministic updates to game state */
    const double time_per_update{0.016666667};

    /** Get current game fps */
    double getFPS() {return 1.0/last_frame_time;}

    bool quit = false;

};

#endif // GAME_H
#include <Game.h>
#include <GameWindow.h>
#include <Entity.h>
#include <Prize.h>
#include <Wall.h>
#include <Floor.h>
#include <Portal.h>
#include <Player.h>
#include <Camera.h>
#include <Hud.h>

#include <iostream> // DELETE
#include <glm/gtx/string_cast.hpp>  // DELETE



int Game::run() {

    // Make window, creating an opengl context
    init_window();

    // Make level, player, entities, hud, and camera
    create_game_objects();

    // Initial event poll to throw away starting cursor pos from event queue
    window->poll_events();

    // Here's the actual game loop
    double update_lag = 0.0;
    while (!window->should_close()) {
        double frame_start_time = glfwGetTime(); // seconds
        glm::vec2 frame_start_cursor = window->cursor;

        handle_input();
        
        while (update_lag >= time_per_update) {
            // Update game state by one time_per_update

            // First resolve all collisions
            for (auto & entity : entities)
                entity->resolve_collisions();

            // Update all entities
            for (auto & entity : entities)
                entity->update(time_per_update);

            // Delete entities that got marked for deletion
            for (auto iter = entities.begin(); iter!=entities.end(); ){
                if ((*iter)->marked_for_deletion)
                    iter = entities.erase(iter); // It's a unique_ptr, so the entity will get deleted when the vector drops it.
                else
                    ++iter;
            }
            hud->update(); //update hud
            update_lag -= time_per_update;
        }
        
        camera->update(player->get_pos() + glm::vec3(0.0f, player->head_height, 0.0f), last_frame_mouse_delta);

        render();

        // Restarting or pausing game if needed
        if (window->restart_pressed) {
            window->restart_pressed = false;
            reset_game();
            create_game_objects();
        }
        else if (player->won) {
            // TODO: make hud display "you won!" and "press enter to restart" and wait for enter
            reset_game();
            create_game_objects();
        }

        window->poll_events();
        last_frame_mouse_delta = window->cursor - frame_start_cursor;
        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;

    } // end game loop

    return 0;
}

void Game::init_window() {
    window = std::make_unique<GameWindow>(1066, 600, "Garbanzo");
}

void Game::create_game_objects() {
    // Generate level
    level = LevelGen::generate_level();
    level.print();
    std::cout << std::endl;

    // Make player
    player = new Player();
    entities.emplace_back(player);
    const auto spawn_loc = level.get_player_spawn_location();
    player->set_pos( glm::vec3(float(spawn_loc[0])+0.5f, 0.0f, float(spawn_loc[1])+0.5f) );

    // Make portal
    Portal * portal = new Portal(window.get());
    portal->set_pos(glm::vec3(float(spawn_loc[0]),0.0f,float(spawn_loc[1])));
    player->collides_with(portal);
    entities.emplace_back(portal);

    // Make walls, floor, and ceiling, and place prize
    Wall * prototype_wall = new Wall(window.get());
    Floor * prototype_floor = new Floor(window.get(), 1.0f, 1.0f);
    for (int i=0; i<LevelGen::TILEMAP_SIZE; ++i) {
        for (int j=0; j<LevelGen::TILEMAP_SIZE; ++j) {
            auto location = LevelGen::vec(i,j);
            if (!level.get_tile(location)) { // if wall
                place_wall(glm::vec3(float(i), 0.0f, float(j)), prototype_wall);
            }
            else { // if floor
                Floor * floor = new Floor(*prototype_floor);
                floor->set_pos(glm::vec3(float(i),0,float(j)));
                entities.emplace_back(floor);
                Floor * ceiling = new Floor(*prototype_floor);
                ceiling->set_pos(glm::vec3(float(i),Wall::wall_height,float(j)));
                entities.emplace_back(ceiling);
            }
            
            if (level.is_treasure(location)) {
                Prize * prize = new Prize(window.get());
                prize->set_pos(glm::vec3(float(i)+0.5, 1.0f, float(j)+0.5));
                player->collides_with(prize);
                entities.emplace_back(prize);
            }
        }
    }
    for (int i=0; i<LevelGen::TILEMAP_SIZE; ++i) { // outside boudary walls front and back
        place_wall(glm::vec3(float(i), 0.0f, -1.0f), prototype_wall);
        place_wall(glm::vec3(float(i), 0.0f, float(LevelGen::TILEMAP_SIZE)), prototype_wall);
    }
    for (int j=0; j<LevelGen::TILEMAP_SIZE; ++j) { // outside boudary walls left and right
        Wall * wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(-1.0, 0.0f, float(j)));
        player->collides_with(wall);
        entities.emplace_back(wall);
        wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(float(LevelGen::TILEMAP_SIZE), 0.0f, float(j)));
        player->collides_with(wall);
        entities.emplace_back(wall);
    }
    delete prototype_wall;
    delete prototype_floor;

    // Create HUD
    hud = std::make_unique<Hud>(window.get(), player);

    // Initialize camera
    camera = std::make_unique<Camera>(
        player->get_pos(), // position
        glm::vec3(0.0f,0.0f,1.0f), // look direction
        glm::vec3(0.0f,1.0f,0.0f)  // up direction
    );
}

void Game::reset_game() {
    entities.clear();
    Prize::reset_graphics_data();
}


void Game::handle_input() {
    bool walking{};
    glm::vec3 player_move_dir{};
    if (window->key_pressed(GLFW_KEY_W)) {
        player_move_dir+=camera->get_dir();
        walking = true;
    }
    else if (window->key_pressed(GLFW_KEY_S)) {
        player_move_dir-=camera->get_dir();
        walking = true;
    }
    if (window->key_pressed(GLFW_KEY_A)) {
        player_move_dir-=camera->get_right();
        walking = true;
    }
    else if (window->key_pressed(GLFW_KEY_D)) {
        player_move_dir+=camera->get_right();
        walking = true;
    }
    if (walking) {
        player_move_dir = glm::normalize(player_move_dir);
        player->set_walking(player_move_dir);
    }
    else
        player->set_not_walking();
}



void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window->set_view_matrix(camera->get_view_matrix());

    for (auto & entity : entities)
        entity->draw(player->get_pos());
    
    hud->draw(); //draw hud last

    window->swap_buffers();
}


void Game::place_wall(const glm::vec3 & pos, Wall * prototype_wall) {
    Wall * wall = new Wall(*prototype_wall);
    wall->set_pos(glm::vec3(pos));
    player->collides_with(wall);
    entities.emplace_back(wall);
}
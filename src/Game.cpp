#include <Game.h>
#include <GameWindow.h>
#include <Entity.h>
#include <Prize.h>
#include <Wall.h>
#include <Player.h>
#include <Camera.h>

#include <iostream> //DELETE

int Game::run() {

    window = std::make_unique<GameWindow>(1066, 600, "Garbanzo");

    // Create player
    player = new Player();
    entities.emplace_back(player);
    player->pos = glm::vec3(0.0f,0.0f,0.0f);

    // Initialize camera
    camera = std::make_unique<Camera>(
        player->pos, // position
        glm::vec3(0.0f,0.0f,1.0f), // look direction
        glm::vec3(0.0f,1.0f,0.0f)  // up direction
    );

    // Generate level
    level = LevelGen::generate_level();
    level.print(); // DELETE this at some point. Will help to keep for testing.
    Wall * prototype_wall = new Wall(window.get());
    for (int i=0; i<LevelGen::TILEMAP_SIZE; ++i) {
        for (int j=0; j<LevelGen::TILEMAP_SIZE; ++j) {
            auto location = LevelGen::vec(i,j);
            if (level.get_tile(location)) { // if floor
                // TODO
            }
            else { // if wall
                Wall * wall = new Wall(*prototype_wall);
                wall->set_pos(glm::vec3(float(i), 0.0f, float(j)));
                entities.emplace_back(wall);
            }
            
            if (level.is_treasure(location)) {
                Prize * prize = new Prize(window.get());
                prize->set_pos(glm::vec3(float(i)+0.5, 1.0f, float(j)+0.5));
                entities.emplace_back(prize);
            }

            if (level.is_start(location)) {
                // TODO
            }
        }
    }
    delete prototype_wall;




    // Here's the actual game loop
    double update_lag = 0.0;
    while (!window->should_close()) {
        double frame_start_time = glfwGetTime(); // seconds
        glm::vec2 frame_start_cursor = window->cursor;

        handle_input();
        
        while (update_lag >= time_per_update) {
            update();
            update_lag -= time_per_update;
        }
        
        camera->update(player->pos + glm::vec3(0.0f, player->head_height, 0.0f), last_frame_mouse_delta);

        render();

        window->poll_events();
        last_frame_mouse_delta = window->cursor - frame_start_cursor;
        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    return 0;
}


void Game::handle_input() {
    bool walking;
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


void Game::update() {
    for (auto & entity : entities)
        entity->update(time_per_update);
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->set_view_matrix(camera->get_view_matrix());

    for (auto & entity : entities)
        entity->draw();

    window->swap_buffers();
}
#include <Game.h>
#include <GameWindow.h>
#include <Entity.h>
#include <Prize.h>
#include <Wall.h>
#include <Floor.h>
#include <Player.h>
#include <Camera.h>

#include <iostream> // DELETE
#include <glm/gtx/string_cast.hpp>  // DELETE

int Game::run() {

    window = std::make_unique<GameWindow>(1066, 600, "Garbanzo");

    // Generate level
    level = LevelGen::generate_level();
    level.print();

    // Make walls, floor, and ceiling
    Wall * prototype_wall = new Wall(window.get());
    Floor * prototype_floor = new Floor(window.get(), 1.0f, 1.0f);
    for (int i=0; i<LevelGen::TILEMAP_SIZE; ++i) {
        for (int j=0; j<LevelGen::TILEMAP_SIZE; ++j) {
            auto location = LevelGen::vec(i,j);
            if (!level.get_tile(location)) { // if wall
                Wall * wall = new Wall(*prototype_wall);
                wall->set_pos(glm::vec3(float(i), 0.0f, float(j)));
                entities.emplace_back(wall);
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
                entities.emplace_back(prize);
            }

            if (level.is_start(location)) {
                // TODO: make the entrance/exit look special
            }
        }
    }
    for (int i=0; i<LevelGen::TILEMAP_SIZE; ++i) { // outside boudary walls front and back
        Wall * wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(float(i), 0.0f, -1.0f));
        entities.emplace_back(wall);
        wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(float(i), 0.0f, float(LevelGen::TILEMAP_SIZE)));
        entities.emplace_back(wall);
    }
    for (int j=0; j<LevelGen::TILEMAP_SIZE; ++j) { // outside boudary walls left and right
        Wall * wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(-1.0, 0.0f, float(j)));
        entities.emplace_back(wall);
        wall = new Wall(*prototype_wall);
        wall->set_pos(glm::vec3(float(LevelGen::TILEMAP_SIZE), 0.0f, float(j)));
        entities.emplace_back(wall);
    }
    delete prototype_wall;
    delete prototype_floor;


    // Make player
    player = new Player();
    entities.emplace_back(player);
    const auto spawn_loc = level.get_player_spawn_location();
    player->pos = glm::vec3(float(spawn_loc[0]), 0.0f, float(spawn_loc[1]));

    // Initialize camera
    camera = std::make_unique<Camera>(
        player->pos, // position
        glm::vec3(0.0f,0.0f,1.0f), // look direction
        glm::vec3(0.0f,1.0f,0.0f)  // up direction
    );

    // Initial event poll to throw away starting cursor pos from event queue
    window->poll_events();

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window->set_view_matrix(camera->get_view_matrix());

    for (auto & entity : entities)
        entity->draw(player->pos);

    window->swap_buffers();
}
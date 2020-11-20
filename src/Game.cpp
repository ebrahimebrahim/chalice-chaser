#include <Game.h>
#include <GameWindow.h>
#include <Entity.h>
#include <Prize.h>
#include <Player.h>
#include <Camera.h>


int Game::run() {

    window = std::make_unique<GameWindow>(800, 600, "Garbanzo");


    // Initialize camera
    camera = std::make_unique<Camera>(
        glm::vec3(0.0f,0.0f,3.0f), // position
        glm::vec3(0.0f,0.0f,0.0f), // look target
        glm::vec3(0.0f,1.0f,0.0f)  // up direction
    );

    // Create entities (TODO the "new" and then sending into unique_ptr is kinda tacky, maybe a factory instead)
    for (int i=0; i<10; i++) {
        for (int j=0; j<10; j++){
            Prize * prize = new Prize(window.get());
            prize->pos = glm::vec3(i,0,j);
            entities.emplace_back(prize);
        }
    }
    player = new Player(window.get());
    entities.emplace_back(player);
    player->pos = camera->get_pos();

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
        
        camera->update(player->pos, last_frame_mouse_delta);

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
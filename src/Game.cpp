#include <Game.h>
#include <GameWindow.h>
#include <Entity.h>
#include <Prize.h>
#include <Player.h>
#include <Camera.h>


int Game::run() {

    window = std::make_unique<GameWindow>(800, 600, "Garbanzo");

    // I'd rather not touch the underlying GLFWwindow here.
    // But how else can I set the key callback, short of passing this entire Game into GameWindow?
    window->set_user_ptr(this);
    window->set_key_callback(
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
            Game * game = static_cast<Game*>(glfwGetWindowUserPointer(window));
            game->handle_key(key, scancode, action, mods); // just forward event to Game::handle_key
        }
    );

    // Create entities
    entities.emplace_back(new Prize(window.get()));
    entities.emplace_back(new Player(window.get()));
    player = static_cast<Player*>( entities.back().get() ); // set handle to player
    player->pos = glm::vec3(0.0f,0.0f,3.0f);

    // Here's the actual game loop
    double update_lag = 0.0;
    while (!quit) {
        double frame_start_time = glfwGetTime(); // seconds

        handle_input();
        
        while (update_lag >= time_per_update) {
            update();
            update_lag -= time_per_update;
        }
        
        render();

        window->poll_events();
        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    return 0;
}


void Game::handle_input() {
    bool walking;
    glm::vec3 player_move_dir{};
    if (window->key_pressed(GLFW_KEY_W)) {
        player_move_dir+=camera.dir;
        walking = true;
    }
    if (window->key_pressed(GLFW_KEY_S)) {
        player_move_dir-=camera.dir;
        walking = true;
    }
    if (window->key_pressed(GLFW_KEY_A)) {
        player_move_dir-=camera.right;
        walking = true;
    }
    if (window->key_pressed(GLFW_KEY_D)) {
        player_move_dir+=camera.right;
        walking = true;
    }
    if (walking) {
        player_move_dir = glm::normalize(player_move_dir);
        player->set_walking(player_move_dir);
    }
    else
        player->set_not_walking();
}

void Game::handle_key(int key, int scancode, int action, int mods) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
        quit = true;
}

void Game::update() {
    if (window->should_close())
        quit = true;
    for (auto & entity : entities)
        entity->update(time_per_update);
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->set_view_matrix(camera.get_view_matrix());

    for (auto & entity : entities)
        entity->draw();

    window->swap_buffers();
}
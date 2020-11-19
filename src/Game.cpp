#include <Game.h>
#include <GameWindow.h>


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

}

void Game::handle_key(int key, int scancode, int action, int mods) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
        quit = true;
}

void Game::update() {
    if (window->should_close())
        quit = true;
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->swap_buffers();
}
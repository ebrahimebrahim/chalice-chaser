#include <Game.h>
#include <thread>
#include <exception>


int Game::run() {
    
    init();

    std::chrono::nanoseconds update_lag{0};
    while (!quit) {
        auto frame_start_time = std::chrono::steady_clock::now();

        handle_input();
        
        while (update_lag >= time_per_update) {
            update();
            update_lag -= time_per_update;
        }
        
        
        render();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - frame_start_time);
        if (elapsed_time < min_time_per_frame){
            std::this_thread::sleep_for(min_time_per_frame - elapsed_time);
        }
        elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - frame_start_time);
        update_lag += elapsed_time;
        fps = double(1e9)/elapsed_time.count();
    }

    uninit();

    return 0;
}

void glfw_error_callback(int error, const char * description) {
    std::string error_header("GLFW Error: ");
    throw std::runtime_error(error_header + description);
}

void Game::init() {
    if (!glfwInit())
        throw std::runtime_error("Unable to initialize GLFW");

    glfwSetErrorCallback(glfw_error_callback);

    // We don't plan to use compatibility features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Need at least OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(window_width, window_height, "Garbanzo", NULL, NULL);
    if (!window)
        throw std::runtime_error("GLFW is either unable to create a window or unable to create an opengl context");
    
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0,0,window_width,window_height);
}

void Game::uninit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::handle_input() {
    glfwPollEvents();
}

void Game::update() {
    if (glfwWindowShouldClose(window))
        quit = true;
}

void Game::render() {

}
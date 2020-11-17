#include <Game.h>
#include <stdexcept>
#include <string>


int Game::run() {
    
    init();

    double update_lag = 0.0;
    while (!quit) {
        double frame_start_time = glfwGetTime(); // seconds

        handle_input();
        
        while (update_lag >= time_per_update) {
            update();
            update_lag -= time_per_update;
        }
        
        render();

        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    uninit();

    return 0;
}

void glfw_error_callback(int error, const char * description) {
    std::string error_header("GLFW Error: ");
    throw std::runtime_error(error_header + description);
}

void glfw_resize_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
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

    glfwSwapInterval(1); // vsync
    glViewport(0,0,window_width,window_height); // tell opengl how to scale its internal coords to window coords
    glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

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


    glfwSwapBuffers(window);
}
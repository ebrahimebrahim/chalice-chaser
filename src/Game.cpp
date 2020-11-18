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



void Game::init() {
    if (!glfwInit())
        throw std::runtime_error("Unable to initialize GLFW");

    glfwSetErrorCallback(
        [](int error, const char * description){
            std::string error_header("GLFW Error: ");
            throw std::runtime_error(error_header + description);
        }
    );

    // We don't plan to use compatibility features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Need at least OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(window_width, window_height, "Garbanzo", NULL, NULL);
    if (!window)
        throw std::runtime_error("GLFW is either unable to create a window or unable to create an opengl context");
    glfwSetWindowUserPointer(window,this); // This is so that glfw callbacks have access to this Game
    
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1); // vsync
    glViewport(0,0,window_width,window_height); // tell opengl how to scale its internal coords to window coords

    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {
            glViewport(0,0,width,height);
        }
    );

    glfwSetKeyCallback(window, 
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
            Game * game = static_cast<Game*>(glfwGetWindowUserPointer(window));
            game->handle_key(key, scancode, action, mods); // just forward event to Game::handle_key
        }
    );

}

void Game::uninit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::handle_input() {
    glfwPollEvents();
}

void Game::handle_key(int key, int scancode, int action, int mods) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
        glfwSetWindowShouldClose(window, 1);
}

void Game::update() {
    if (glfwWindowShouldClose(window))
        quit = true;
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
}
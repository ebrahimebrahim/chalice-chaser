#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>

const int window_width = 800;
const int window_height = 600;
double time_per_update = 0.016666667;

int main() {

    // Initialize GLFW and OpenGL context

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
    GLFWwindow * window = glfwCreateWindow(window_width, window_height, "Garbanzo", NULL, NULL);
    if (!window)
        throw std::runtime_error("GLFW is either unable to create a window or unable to create an opengl context");
    
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
            if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
                glfwSetWindowShouldClose(window, 1);
        }
    );

    // Game loop

    bool quit = false;
    double update_lag = 0.0;
    double last_frame_time{};
    while (!quit) {
        double frame_start_time = glfwGetTime(); // seconds

        glfwPollEvents(); // Handle input
        
        // Update game state
        while (update_lag >= time_per_update) {
            if (glfwWindowShouldClose(window))
                quit = true;
            update_lag -= time_per_update;
        }
        
        // ------------ Rendering ------------

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        
        // ------------ End Rendering ------------

        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
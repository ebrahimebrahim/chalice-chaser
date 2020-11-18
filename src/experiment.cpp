#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <Shader.h>

#define BUFFER_OFFSET(offset) ((void *)(offset))

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

    // The "loading phase" for various graphics objects, let's call them "walls" and "prize"

    // First, prize
    const int prize_num_verts = 5;
    static const GLfloat prize_vertices[prize_num_verts][3] = {
        { 0.0f, 0.0f, 0.0f }, // center of fan
        { -0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, 0.3f },
        { -0.3f, -0.3f, 0.3f },
    };
    GLuint prize_vbo{};
    glCreateBuffers(1,&prize_vbo);
    glNamedBufferStorage(prize_vbo, sizeof(prize_vertices), prize_vertices, 0);

    GLuint prize_vao{};
    glCreateVertexArrays(1,&prize_vao);
    glBindVertexArray(prize_vao);
    glBindBuffer(GL_ARRAY_BUFFER, prize_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    Shader * prize_shader = new Shader("src/shader.vert","src/shader.frag");


    // Next, walls
    const int walls_num_verts = 6;
    static const GLfloat walls_vertices[walls_num_verts][3] = {
        { -0.5f,  0.5f, -0.5f }, 
        { -0.5f, -0.5f, -0.5f },
        { -0.4f,  0.5f,  0.5f },
        { -0.4f, -0.5f,  0.5f },
        { -0.0f,  0.5f,  0.7f },
        { -0.0f, -0.5f,  0.7f },
    };
    GLuint walls_vbo{};
    glCreateBuffers(1,&walls_vbo);
    glNamedBufferStorage(walls_vbo, sizeof(walls_vertices), walls_vertices, 0);

    GLuint walls_vao{};
    glCreateVertexArrays(1,&walls_vao);
    glBindVertexArray(walls_vao);
    glBindBuffer(GL_ARRAY_BUFFER, walls_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    Shader * walls_shader = new Shader("src/shader.vert","src/shader.frag");



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

        // Render prize
        prize_shader->use();
        glBindVertexArray(prize_vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, prize_num_verts);
        
        // Render walls
        walls_shader->use();
        glBindVertexArray(walls_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, walls_num_verts);

        glfwSwapBuffers(window);
        
        // ------------ End Rendering ------------

        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    delete prize_shader;
    delete walls_shader;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <Shader.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>


#define BUFFER_OFFSET(offset) ((void *)(offset))

const int window_width = 800;
const int window_height = 600;
double time_per_update = 0.016666667;
glm::vec2 cursor;

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


    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window,
        [](GLFWwindow* window, double xpos, double ypos){
            cursor = glm::vec2(xpos,ypos);
        }
    );
    cursor = glm::vec2(window_width/2,window_height/2);


    // -------- The "loading phase" for various graphics objects, let's call them "walls" and "prize"  -------

    // First, prize
    GLuint prize_vao{};
    glGenVertexArrays(1,&prize_vao);
    glBindVertexArray(prize_vao);
    
    const int prize_num_verts = 5;
    const GLfloat prize_vertices[prize_num_verts][3] = {
        { 0.0f, 0.0f, 0.0f }, // center of fan
        { -0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, -0.3f },
        { 0.3f, -0.3f, 0.3f },
        { -0.3f, -0.3f, 0.3f },
    };
    GLuint prize_vbo{};
    glGenBuffers(1,&prize_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, prize_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(prize_vertices), prize_vertices,  GL_STATIC_DRAW);

    const int prize_num_indices = 5;
    const GLuint prize_indices[prize_num_indices] = { 0,1,2,3,4 };
    GLuint prize_ebo{};
    glGenBuffers(1, &prize_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prize_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prize_indices), prize_indices,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    Shader * prize_shader = new Shader("src/shader.vert","src/shader.frag");


    // Next, walls
    GLuint walls_vao{};
    glGenVertexArrays(1,&walls_vao);
    glBindVertexArray(walls_vao);
    
    const int walls_num_verts = 6;
    const GLfloat walls_vertices[walls_num_verts][3] = {
        { -0.5f,  0.5f, -0.5f }, 
        { -0.5f, -0.5f, -0.5f },
        { -0.4f,  0.5f,  0.5f },
        { -0.4f, -0.5f,  0.5f },
        { -0.0f,  0.5f,  0.7f },
        { -0.0f, -0.5f,  0.7f },
    };
    GLuint walls_vbo{};
    glGenBuffers(1,&walls_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, walls_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(walls_vertices), walls_vertices, GL_STATIC_DRAW);

    const int walls_num_indices = 6;
    const GLuint walls_indices[walls_num_indices] = { 0,1,2,3,4,5 };
    GLuint walls_ebo{};
    glGenBuffers(1, &walls_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, walls_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(walls_indices), walls_indices,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    Shader * walls_shader = new Shader("src/shader.vert","src/shader.frag");

    // ------------ end graphics object loading phase ----------------



    // -- Setting up a camera, model, view, projection

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),float(window_width)/float(window_height),0.1f,100.0f);

    auto cameraPos = glm::vec3(0.0f,0.0f,3.0f); // cam origin
    auto cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
    auto cameraNegDirection = glm::normalize(cameraPos - cameraTarget); // cam z axis
    auto cameraDir = - cameraNegDirection;
    glm::mat4 view = glm::lookAt(cameraPos,cameraTarget,glm::vec3(0.0,1.0,0.0));

    prize_shader->use(); // note that use needs to be called before setting uniforms!
    prize_shader->setUniform("projection",projection);
    walls_shader->use();
    walls_shader->setUniform("projection",projection);

    // -- End setting up camera, model, view, projection



    // Game loop

    bool quit = false;
    double update_lag = 0.0;
    double last_frame_time{};
    glm::vec2 last_frame_mouse_delta{};
    while (!quit) {
        double frame_start_time = glfwGetTime(); // seconds
        glm::vec2 frame_start_cursor = cursor;

        // ------------ Handle input ------------

        
        float walkSpeed = 2.0f;
        float mouseSensitivity = 0.0015f;
        float walkDist = last_frame_time * walkSpeed;
        glm::vec3 camRight = glm::normalize(glm::cross(cameraDir,glm::vec3(0.0,1.0,0.0)));
        glm::vec3 worldUp  = glm::vec3(0.0f,1.0f,0.0f);
        if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += walkDist * cameraDir;
        if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= walkDist * cameraDir;
        if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
            glm::vec3 camRight = glm::normalize(glm::cross(cameraDir,worldUp));
            cameraPos -= walkDist * camRight;
        }
        if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
            cameraPos += walkDist * camRight;
        }

        // set yaw
        cameraDir = glm::rotate(cameraDir,-mouseSensitivity*last_frame_mouse_delta[0],worldUp);

        //set pitch
        glm::vec3 new_dir = glm::rotate(cameraDir,-mouseSensitivity*last_frame_mouse_delta[1],camRight);
        if (new_dir[0]*cameraDir[0] + new_dir[2]*cameraDir[2] > 0.1f) // don't allow pitch beyond zenith/nadir
            cameraDir = new_dir;
        
        view = glm::lookAt(cameraPos,cameraPos+cameraDir,glm::vec3(0.0,1.0,0.0));

        
        // ------------ End input handling ------------


        // Update game state
        while (update_lag >= time_per_update) {
            if (glfwWindowShouldClose(window))
                quit = true;
            



            update_lag -= time_per_update;
        }
        
        // ------------ Rendering ------------

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Upate view matrix
        view = glm::lookAt(cameraPos,cameraPos+cameraDir,glm::vec3(0.0,1.0,0.0));

        // Render prize
        prize_shader->use();
        prize_shader->setUniform("model",model);
        prize_shader->setUniform("view",view);
        glBindVertexArray(prize_vao);
        glDrawElements(GL_TRIANGLE_FAN, prize_num_indices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        
        // Render walls
        walls_shader->use();
        walls_shader->setUniform("model",model);
        walls_shader->setUniform("view",view);
        glBindVertexArray(walls_vao);
        glDrawElements(GL_TRIANGLE_STRIP, walls_num_indices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        glfwSwapBuffers(window);
        
        // ------------ End Rendering ------------

        glfwPollEvents(); //  I think this should be an exposed part of the game loop, not tucked into handle_input
        last_frame_mouse_delta = cursor - frame_start_cursor;
        last_frame_time = glfwGetTime() - frame_start_time;
        update_lag += last_frame_time;
    }

    delete prize_shader;
    delete walls_shader;

    glDeleteVertexArrays(1,&prize_vao);
    glDeleteBuffers(1,&prize_ebo);
    glDeleteBuffers(1,&prize_vbo);

    glDeleteVertexArrays(1,&walls_vao);
    glDeleteBuffers(1,&walls_ebo);
    glDeleteBuffers(1,&walls_vbo);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
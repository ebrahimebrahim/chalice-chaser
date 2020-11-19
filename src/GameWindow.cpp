#include <GameWindow.h>
#include <stdexcept>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

GameWindow::GameWindow(int width, int height, const char * title) {
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
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
        throw std::runtime_error("GLFW is either unable to create a window or unable to create an opengl context");
    
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1); // vsync
    glViewport(0,0,width,height); // tell opengl how to scale its internal coords to window coords

    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {
            glViewport(0,0,width,height);
        }
    );

    GraphicsObject::projection = glm::perspective(glm::radians(45.0f),float(width)/float(height),0.1f,100.0f);
}

GameWindow::GameWindow(GameWindow && src) {
    width = src.width;
    height = src.height;
    window = src.window;
    src.window = nullptr;
}

GameWindow & GameWindow::operator=(GameWindow && src) {
    if (&src == this) return *this;
    width = src.width;
    height = src.height;
    window = src.window;
    src.window = nullptr;
    return *this;
}


void GameWindow::set_user_ptr(void * ptr) {
    glfwSetWindowUserPointer(window,ptr);
}

void GameWindow::set_key_callback(GLFWkeyfun callback){
    glfwSetKeyCallback(window, callback);
}


GameWindow::~GameWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

glm::mat4 GraphicsObject::projection{};

GraphicsObject::GraphicsObject(const GraphicsData & graphics_data) :
        num_indices{graphics_data.num_indices},
        draw_mode{graphics_data.draw_mode},
        model_matrix{1.0f}
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, graphics_data.num_vertices * sizeof(GLfloat) * 3, graphics_data.vertices,  GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, graphics_data.num_indices * sizeof(GLuint) * 1, graphics_data.indices,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    shader = std::make_unique<Shader>(graphics_data.vertex_shader_path, graphics_data.fragment_shader_path);
    shader->use();
    shader->setUniform("projection",projection);
}

GraphicsObject::GraphicsObject(GraphicsObject && src) {
    vbo = src.vbo;
    ebo = src.ebo;
    vao = src.vao;
    src.vbo = 0;
    src.ebo = 0;
    src.vao = 0;

    draw_mode = src.draw_mode;
    num_indices = src.num_indices;
    shader = std::move(src.shader);
    model_matrix = src.model_matrix;
}

GraphicsObject & GraphicsObject::operator=(GraphicsObject && src) {
    if (&src==this) return *this;

    vbo = src.vbo;
    ebo = src.ebo;
    vao = src.vao;
    src.vbo = 0;
    src.ebo = 0;
    src.vao = 0;

    draw_mode = src.draw_mode;
    num_indices = src.num_indices;
    shader = std::move(src.shader);
    model_matrix = src.model_matrix;

    return *this;
}

void GraphicsObject::draw(const glm::mat4 & view_matrix) const {
    shader->use();
    shader->setUniform("model",model_matrix);
    shader->setUniform("view",view_matrix);
    glBindVertexArray(vao);
    glDrawElements(draw_mode, num_indices, GL_UNSIGNED_INT, 0);
}

GraphicsObject::~GraphicsObject() {
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1,&ebo);
    glDeleteBuffers(1,&vbo);
}
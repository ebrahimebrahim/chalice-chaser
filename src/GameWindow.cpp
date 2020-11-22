#include <GameWindow.h>

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

GameWindow::GameWindow(int width, int height, const char * title) :
    width(width), height(height), cursor(width/2,height/2)
{
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


    glfwSetKeyCallback(window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
            if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
                glfwSetWindowShouldClose(window,GLFW_TRUE);
        }
    );

    glfwSetWindowUserPointer(window, this);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    else
        std::cout << "Warning: Raw mouse motion is not supported.\n";
    glfwSetCursorPosCallback(window,
        [](GLFWwindow* window, double xpos, double ypos){
            GameWindow * gw = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
            gw->cursor = glm::vec2(xpos,ypos);
        }
    );

    shaders[ShaderChoice::DEFAULT] = new Shader("src/shader.vert", "src/shader.frag");

    set_projection_matrix( glm::perspective(glm::radians(45.0f),float(width)/float(height),0.1f,100.0f) );
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

GameWindow::~GameWindow() {
    for (auto & pair : id_to_graphics_object)
        delete pair.second;
    for (auto & pair : shaders)
        delete pair.second;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GameWindow::add_object(int id, const GraphicsData & graphics_data) {
    if (id_to_graphics_object.find(id)!= id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object with id " << id << " is already registered, but the program is trying to add it!";
        throw std::runtime_error(error_msg.str());
    }
    id_to_graphics_object[id] = new GraphicsObject(graphics_data, shaders[graphics_data.shader_choice]);
}

void GameWindow::del_object(int id) {
    if (id_to_graphics_object.find(id)== id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object associated to id " << id << " is to be deleted, but no such association exists!";
        throw std::runtime_error(error_msg.str());
    }
    delete id_to_graphics_object[id];
    id_to_graphics_object.erase(id);
}

void GameWindow::draw(int id) {
    if (id_to_graphics_object.find(id)== id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object associated to id " << id << " is being requested, but no such association exists!";
        throw std::runtime_error(error_msg.str());
    }
    id_to_graphics_object[id]->draw();
}

void GameWindow::set_view_matrix(const glm::mat4 & view) {
    view_matrix = view;
    for (auto & pair : shaders){
        pair.second->use();
        pair.second->setUniform("view", view_matrix);
    }
}

void GameWindow::set_projection_matrix(const glm::mat4 & projection) {
    projection_matrix = projection;
    for (auto & pair : shaders){
        pair.second->use();
        pair.second->setUniform("projection", projection_matrix);
    }
}

void GameWindow::set_object_model_matrix(int id, const glm::mat4 & model) {
    if (id_to_graphics_object.find(id) == id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object associated to id " << id << " is being requested to set a model matrix, but no such id association exists!";
        throw std::runtime_error(error_msg.str());
    }
    id_to_graphics_object[id]->set_model_matrix(model);
}


GraphicsObject::GraphicsObject(const GraphicsData & graphics_data, Shader * shader) :
        num_indices{graphics_data.num_indices},
        draw_mode{graphics_data.draw_mode},
        model_matrix{1.0f},
        shader(shader)
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

void GraphicsObject::draw() const {
    shader->use();
    shader->setUniform("model",model_matrix);
    glBindVertexArray(vao);
    glDrawElements(draw_mode, num_indices, GL_UNSIGNED_INT, 0);
}

GraphicsObject::~GraphicsObject() {
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1,&ebo);
    glDeleteBuffers(1,&vbo);
}
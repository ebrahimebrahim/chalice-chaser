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

GameWindow::GameWindow(int windowed_width, int windowed_height, const char * title) :
    windowed_width(windowed_width), windowed_height(windowed_height), cursor(0, 0)
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


    monitor = glfwGetPrimaryMonitor();
    video_mode = glfwGetVideoMode(monitor);
 
    glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

    fullscreen = true;
    window = glfwCreateWindow(video_mode->width, video_mode->height, title, monitor, NULL);
    if (!window)
        throw std::runtime_error("GLFW is either unable to create a window or unable to create an opengl context");
    
    glfwMakeContextCurrent(window);


    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1); // vsync
    glViewport(0,0,get_width(),get_height()); // tell opengl how to scale its internal coords to window coords

    
    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {
            glViewport(0,0,width, height);  
        }
    );

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
            if (action != GLFW_RELEASE) return;
            GameWindow * gw = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
            switch(key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window,GLFW_TRUE); return;
                case GLFW_KEY_F:
                    gw->switch_fullscreen();
                    return;
                case GLFW_KEY_R:
                    gw->restart_pressed = true;
                    return;
                case GLFW_KEY_P:
                    gw->pause_pressed = true;
                    return;
            }
        }
    );


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

    shaders[SHADER_DEFAULT] = new Shader("src/shaders/shader.vert", "src/shaders/shader.frag");
    shaders[SHADER_HUD] = new Shader("src/shaders/hud_shader.vert", "src/shaders/hud_shader.frag");
    shaders[SHADER_PORTAL] = new Shader("src/shaders/portal_shader.vert", "src/shaders/portal_shader.frag");
    shaders[SHADER_TEXTURE] = new Shader("src/shaders/shader_texture.vert", "src/shaders/shader_texture.frag");
    shaders[SHADER_HUD_TEXTURE] = new Shader("src/shaders/hud_shader_texture.vert", "src/shaders/hud_shader_texture.frag");

    calculate_projection_matrix();

    glEnable(GL_DEPTH_TEST); 
}

GameWindow::GameWindow(GameWindow && src) {
    windowed_width = src.windowed_width;
    windowed_height = src.windowed_height;
    window = src.window;
    src.window = nullptr;
}

GameWindow & GameWindow::operator=(GameWindow && src) {
    if (&src == this) return *this;
    windowed_width = src.windowed_width;
    windowed_height = src.windowed_height;
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

void GameWindow::calculate_projection_matrix() {
    set_projection_matrix( glm::perspective(glm::radians(45.0f),float(get_width())/float(get_height()),0.1f,100.0f) );
}

void GameWindow::switch_fullscreen() {
    if (fullscreen) {
        fullscreen = false;
        glfwSetWindowMonitor(window, NULL, 200, 200, windowed_width, windowed_height, video_mode->refreshRate);
        calculate_projection_matrix();
        glfwRestoreWindow(window); // switching from windowed to fullscreen ends up maximized for some reason, so restore
    }
    else {
        fullscreen = true;
        glfwSetWindowMonitor(window, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate);
        calculate_projection_matrix();
    }
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

void GameWindow::duplicate_object(int id_original, int id_copy) {
    if (id_to_graphics_object.find(id_original)== id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "There's no graphics object with id " << id_original << ", but the program is trying to duplicate it!";
        throw std::runtime_error(error_msg.str());
    }
    if (id_to_graphics_object.find(id_copy)!= id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object with id " << id_copy << " already exists, but the program is trying to add it via duplication!";
        throw std::runtime_error(error_msg.str());
    }
    id_to_graphics_object[id_copy] = new GraphicsObject(*id_to_graphics_object[id_original]);
}

void GameWindow::change_object_id(int id_old, int id_new) {
    if (id_to_graphics_object.find(id_old)== id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "There's no graphics object with id " << id_old << ", but the program is trying to change its id!";
        throw std::runtime_error(error_msg.str());
    }
    if (id_to_graphics_object.find(id_new)!= id_to_graphics_object.end()){
        std::ostringstream error_msg;
        error_msg << "A graphics object with id " << id_new << " already exists, but the program is trying to a rename something to it!";
        throw std::runtime_error(error_msg.str());
    }
    id_to_graphics_object[id_new] = id_to_graphics_object[id_old];
    id_to_graphics_object.erase(id_old);
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
    for (auto shader_index : {SHADER_DEFAULT, SHADER_PORTAL, SHADER_TEXTURE}){
        shaders[shader_index]->use();
        shaders[shader_index]->set_uniform("view", view_matrix);
    }
}

void GameWindow::set_projection_matrix(const glm::mat4 & projection) {
    projection_matrix = projection;
    for (auto shader_index : {SHADER_DEFAULT, SHADER_PORTAL, SHADER_TEXTURE}){
        shaders[shader_index]->use();
        shaders[shader_index]->set_uniform("projection", projection_matrix);
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


GraphicsObjectBufferData::GraphicsObjectBufferData(const GraphicsData & graphics_data) :
    num_indices{graphics_data.indices.size()},
    draw_mode{graphics_data.draw_mode}
{
    if ( !graphics_data.texture && graphics_data.vertices.size()%3 )
        throw std::runtime_error("Invalid vector of vertices passed to GraphicObjectBufferData. Each three elements should represent one vertex.");
    else if ( graphics_data.texture && graphics_data.vertices.size()%5 )
        throw std::runtime_error("Invalid vector of vertices passed to GraphicObjectBufferData. Each five elements should represent a vertex and its texture coord.");


    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, graphics_data.vertices.size() * sizeof(GLfloat), graphics_data.vertices.data(),  GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, graphics_data.indices.size() * sizeof(GLuint) * 1, graphics_data.indices.data(),  GL_STATIC_DRAW);

    if (graphics_data.texture) {
        glGenTextures(1,&tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // needed because stb_image doesn't perform any row alignment of pixel data. see https://www.khronos.org/opengl/wiki/Pixel_Transfer#Pixel_layout
        assert(graphics_data.texture->get_num_channels()==3);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            graphics_data.texture->get_width(), graphics_data.texture->get_height(), 0,
            GL_RGB ,
            GL_UNSIGNED_BYTE, graphics_data.texture->get_data()
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*) (0*sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*) (3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    else {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

}

GraphicsObjectBufferData::GraphicsObjectBufferData(GraphicsObjectBufferData && src) {
    vbo = src.vbo;
    ebo = src.ebo;
    vao = src.vao;
    tex = src.tex;
    src.vbo = 0;
    src.ebo = 0;
    src.vao = 0;
    src.tex = 0;

    draw_mode = src.draw_mode;
    num_indices = src.num_indices;
}

GraphicsObjectBufferData & GraphicsObjectBufferData::operator=(GraphicsObjectBufferData && src) {
    if (&src==this) return *this;

    vbo = src.vbo;
    ebo = src.ebo;
    vao = src.vao;
    tex = src.tex;
    src.vbo = 0;
    src.ebo = 0;
    src.vao = 0;
    src.tex = 0;

    draw_mode = src.draw_mode;
    num_indices = src.num_indices;

    return *this;
}

GraphicsObjectBufferData::~GraphicsObjectBufferData() {
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1,&ebo);
    glDeleteBuffers(1,&vbo);
    if (tex!=0) glDeleteTextures(1,&tex);
}

void GraphicsObjectBufferData::bind_vao_and_draw() const {
    if (tex!=0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
    glBindVertexArray(vao);
    glDrawElements(draw_mode, num_indices, GL_UNSIGNED_INT, 0);
}




GraphicsObject::GraphicsObject(const GraphicsData & graphics_data, Shader * shader) :
        
        model_matrix{1.0f},
        shader(shader),
        object_color(graphics_data.object_color)
{
    buffer_data = std::make_shared<GraphicsObjectBufferData>(graphics_data);
}

GraphicsObject::GraphicsObject(const GraphicsObject & src) {
    buffer_data = src.buffer_data; // copy assignment of shared ptr, increases ref count
    shader = src.shader; // copy handle to shader, which is not an owned resource
    model_matrix = src.model_matrix; // copy
    object_color = src.object_color;

}

GraphicsObject & GraphicsObject::operator=(const GraphicsObject & src) {
    if (&src==this) return *this;

    buffer_data = src.buffer_data;
    shader = src.shader;
    model_matrix = src.model_matrix;
    object_color = src.object_color;


    return *this;
}

GraphicsObject::GraphicsObject(GraphicsObject && src) {
    buffer_data = std::move(src.buffer_data);
    shader = src.shader;
    model_matrix = src.model_matrix;
    object_color = src.object_color;

    src.shader = nullptr;
}

GraphicsObject & GraphicsObject::operator=(GraphicsObject && src) {
    if (&src==this) return *this;

    buffer_data = std::move(src.buffer_data);
    shader = src.shader;
    model_matrix = src.model_matrix;
    object_color = src.object_color;

    src.shader = nullptr;

    return *this;
}

void GraphicsObject::draw() const {
    shader->use();
    shader->set_uniform("model",model_matrix);
    if (object_color) shader->set_uniform("object_color", *object_color);
    buffer_data->bind_vao_and_draw();
}
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <memory>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <glm/glm.hpp>

class GraphicsObject;
struct GraphicsData;

/**
 *  Holds a GLFW window and associated OpenGL context
 */
class GameWindow {

    GLFWwindow * window{};
    int width{};
    int height{};
    std::unordered_map<int,GraphicsObject*> id_to_graphics_object;

public:
    GameWindow(int width, int height, const char * title);
    ~GameWindow();
    GameWindow(const GameWindow &) = delete; // no copy
    GameWindow & operator=(const GameWindow &) = delete;
    GameWindow(GameWindow &&); /** move is okay, passes ownership of window */
    GameWindow & operator=(GameWindow &&);


    /** Set the key callback for this window; lookup glfwSetKeyCallback for expected function */
    void set_key_callback(GLFWkeyfun);

    /** Set a pointer that will be attached to the GLFWwindow.
     * When defining a callback, you will have access to the underlying GLFWwindow.
     * Then you can get whatever pointer you set using glfwGetWindowUserPointer
     */
    void set_user_ptr(void *);

    /** Return the value of the GLFW close flag of the window  */
    int should_close() const {return glfwWindowShouldClose(window);}

    void swap_buffers() {glfwSwapBuffers(window);}

    /** Wrap glfwPollEvents */
    void poll_events() {glfwPollEvents();}
    
    int get_width() const {return width;}
    int get_height() const {return height;}

    /** Add an object with a given id, creating the GraphicsObject.
     *  Once you do this, it becomes possible to call `draw` and pass in the id.
     *  Throws a runtime error if an object of the given id was already added.
     *  Whoever called add_object does not need to worry about deleting the object.
     *  Only delete if you want the object to disappear before this GameWindow dies.
     */
    void add_object(int id, const GraphicsData & graphics_data);

    /**Destroy an object, freeing it's data in video memory. 
     * Throws a runtime error if no object with the given id was ever added.
     * Only do this if you want the object to disappear before this GameWindow dies.
     */
    void del_object(int id);


    /** Render the object of the given id to the framebuffer.
     *  Throws a runtime error if an object of the given id was never added.
     */
    void draw(int id);

    /**Set the view matrix that transforms world coordinates to camera coordinates.
     * It will be used for all rendering.
     */
    void set_view_matrix(const glm::mat4 & view);

    /**Set the perspective projection matrix, to be used for all rendering  */
    void set_projection_matrix(const glm::mat4 & projection);

    /**Set the model matrix that transforms object coordinates to world coordinates.
     * `id` is the id of the object; runtime error thrown if no object with this id
     * was ever added via `add_object`
     */
    void set_object_model_matrix(int id, const glm::mat4 & model);

};

/**
 * A specification of a graphics object and how to draw it,
 * mainly to be passed into the constructor of GraphicsObject.
 */
struct GraphicsData{
    GLfloat * vertices;
    GLsizei num_vertices;
    GLuint * indices;
    GLsizei num_indices;
    GLenum draw_mode;
    const char * vertex_shader_path;
    const char * fragment_shader_path;
};


/**
 * Represents and manages the data of a graphics object in the opengl buffers,
 * as well as information about how to draw it.
 * On destruction, GraphicsObjects will clean up the buffers and shaders they manage,
 * so make sure to destroy your GraphicsObjects before the opengl context is removed from under them.
*/
class GraphicsObject {

    friend GameWindow;

    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    
    GLenum draw_mode{}; /** e.g. GL_TRIANGLES */
    GLsizei num_indices{}; /** Number of indices in EBO */
    
    std::unique_ptr<Shader> shader;
    glm::mat4 model_matrix{}; /** The matrix that transforms object coords to world coords, placing an object in the world. */

    static glm::mat4 projection_matrix; /** Perspective projection matrix. */
    static glm::mat4 view_matrix; /** The matrix that transforms world coords to camera coords */
public:

    /** 
     * The vertex shader is assumed to have mat4 inputs named "model", "view", and "projection".
    */
    GraphicsObject(const GraphicsData & graphics_data);
    GraphicsObject(const GraphicsObject &) = delete;
    GraphicsObject & operator=(const GraphicsObject &) = delete;
    GraphicsObject(GraphicsObject &&);
    GraphicsObject & operator=(GraphicsObject &&);;
    ~GraphicsObject();

    void draw() const;
    void set_model_matrix(glm::mat4 m) {model_matrix = m;}

};


#endif // GAME_WINDOW_H
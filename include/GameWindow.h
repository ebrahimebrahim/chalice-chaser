#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <StbImage.h>
#include <glm/glm.hpp>

class GraphicsObject;
struct GraphicsData;
enum ShaderChoice {SHADER_DEFAULT, SHADER_HUD, SHADER_PORTAL, SHADER_TEXTURE};

/**
 *  Holds a GLFW window and associated OpenGL context
 */
class GameWindow {

    GLFWwindow * window{};
    int windowed_width{}; /** Width to be used in windowed mode */
    int windowed_height{}; /** Height to be used in windowed mode */
    GLFWmonitor * monitor{}; /** The primary monitor of the user */
    const GLFWvidmode * video_mode{}; /** The video mode of the primary monitor, saved at the start so we can do "windowed fullscreen" */
    bool fullscreen{}; /** Whether we are currently in fullscreen mode */
    std::unordered_map<int,GraphicsObject*> id_to_graphics_object;
    
    /** Shaders are indeed owned by GameWindow.
     * We don't use a smart pointer because we want to carefully control order of deallocation of GameWindow's resources.
     * The first coordinate in this map is a `ShaderChoice`, but it is written as `int` because apparently enums are not hashable
     * in older versions of the standard.
     */
    std::unordered_map<int,Shader*> shaders;

    glm::mat4 projection_matrix; /** Perspective projection matrix. */
    glm::mat4 view_matrix; /** The matrix that transforms world coords to camera coords */


    void switch_fullscreen();
    void calculate_projection_matrix(); /**Compute and set the matrix used for perspective projection*/



public:
    GameWindow(int width, int height, const char * title);
    ~GameWindow();
    GameWindow(const GameWindow &) = delete; // no copy
    GameWindow & operator=(const GameWindow &) = delete;
    GameWindow(GameWindow &&); /** move is okay, passes ownership of window */
    GameWindow & operator=(GameWindow &&);

    /** Return the value of the GLFW close flag of the window  */
    int should_close() const {return glfwWindowShouldClose(window);}

    void swap_buffers() {glfwSwapBuffers(window);}

    /** Wrap glfwPollEvents */
    void poll_events() {glfwPollEvents();}

    /** Return whether the key is pressed in the last reported state of the event queue for the window
     * See this for keys: https://www.glfw.org/docs/latest/group__keys.html */
    bool key_pressed(int key) {return glfwGetKey(window,key) == GLFW_PRESS;}
    
    int get_width() const {return fullscreen? video_mode->width : windowed_width;}
    int get_height() const {return fullscreen? video_mode->height : windowed_height;}

    /** Position of mouse cursor, updated regularly as polling occurs (updates on a glfwSetCursorPosCallback) */
    glm::vec2 cursor;

    /** whether a game restart has been triggered by player input */
    bool restart_pressed{false}; 

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

    /**Copy an object, creating a new object with the same (and in fact shared) vertex data in the opengl buffers.
     * Give the id of the original object (which must already have been added via `add_object`)
     * and give the desired id of the new copy.
    */
    void duplicate_object(int id_original, int id_copy);

    /**Change the id of a previously added object*/
    void change_object_id(int id_old, int id_new);


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

    template <typename T>
    void set_uniform(ShaderChoice shader_choice, const char * name, const T & value) {
        shaders[shader_choice]->set_uniform(name,value);
    }

};

/**
 * A specification of a graphics object and how to draw it,
 * mainly to be passed into the constructor of GraphicsObject.
 */
struct GraphicsData{
    
    /**Each three floats is one 3d vertex... unless a texture is specified (see below),
     * in which case each 5 floats is a 3d vertex followed by a 2d texture coordinate.
     */
    std::vector<GLfloat> vertices;
    
    std::vector<GLuint> indices;
    GLenum draw_mode;
    ShaderChoice shader_choice;

    /** It's optional to specify a texture. If it's left null, then there's no texture. */
    std::shared_ptr<StbImage> texture; 
};

/**
 * Represents and manages the data of a graphics object in the opengl buffers,
 * as well as information about how to draw it.
 * This is purely about the object space, nothing to do with placing the object in the world.
 * On destruction, GraphicsObjectBufferData will clean up the buffers it manages,
 * so make sure to destroy it before the opengl context is removed from under them.
*/
class GraphicsObjectBufferData{
    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    GLuint tex{}; // texture name will be set and used only if there's a texture. 0 means there isn't.
    
    GLenum draw_mode{}; /** e.g. GL_TRIANGLES */
    size_t num_indices{}; /** Number of indices in EBO */

public:

    GraphicsObjectBufferData(const GraphicsData & graphics_data);
    GraphicsObjectBufferData(const GraphicsObjectBufferData &) = delete;
    GraphicsObjectBufferData & operator=(const GraphicsObjectBufferData &) = delete;
    GraphicsObjectBufferData(GraphicsObjectBufferData &&);
    GraphicsObjectBufferData & operator=(GraphicsObjectBufferData &&);;
    ~GraphicsObjectBufferData();

    void bind_vao_and_draw() const;
};

/**
 * Represents and manages the data of a graphics object and how to draw it,
 * including possibly shared opengl buffer data, a choice of shader, and a model matrix that places and object in the world.
*/
class GraphicsObject {
    std::shared_ptr<GraphicsObjectBufferData> buffer_data;
    Shader * shader; // handle to shader, not owned by GraphicsObject
    glm::mat4 model_matrix{}; /** The matrix that transforms object coords to world coords, placing an object in the world. */
    
public:

    GraphicsObject(const GraphicsData & graphics_data, Shader * shader);
    GraphicsObject(const GraphicsObject &);
    GraphicsObject & operator=(const GraphicsObject &);
    GraphicsObject(GraphicsObject &&);
    GraphicsObject & operator=(GraphicsObject &&);
    ~GraphicsObject() {}

    void draw() const;
    void set_model_matrix(glm::mat4 m) {model_matrix = m;}

};


#endif // GAME_WINDOW_H
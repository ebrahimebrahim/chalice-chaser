#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 *  Holds a GLFW window and associated OpenGL context
 */


class GameWindow {

    GLFWwindow * window;
    int width;
    int height;

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
    
    int get_width() const {return width;}
    int get_height() const {return height;}

};

#endif // GAME_WINDOW_H
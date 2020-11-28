# Chalice Chaser

This is my final project for the Udacity C++ course.
This branch of the project is specifically made to run in the Udacity workspace,
which has some limitations.
Look at the main branch for the actual game.

Limitations of the udacity workspace version of the game:
- Arrow keys control camera movement rather than the mouse. This is awkward.
The udacity virtual machine is not happy with having the mouse captured.
- The level generated is very small, so that it can run in the virtual machine.


## Building

Install opengl dependencies on linux:
```
sudo apt-get install libgl1-mesa-dev libglfw3-dev
```

In the Udacity workspace, the repository-provided version of libglfw3 is too old.
I've put a copy of a new glfw. To install it:
```
cd /home/workspace/glfw/build
make install
```
This is done automatically in `/home/workspace/.student_bashrc`.

Finally, build the project:
```
cd /home/workspace/symmetrical-garbanzo
mkdir build
cd build
cmake ..
make
```

## Playing

```
cd /home/workspace/symmetrical-garbanzo
./run.sh
```

| action            | control        |
|-------------------|----------------|
| quit              | esc            |
| move              | WASD           |
| look              | arrow keys     |
| restart           | R              |
| toggle fullscreen | F              |

You will spawn in front of a portal in a randomly generated level.
The level has a treasure in it. Find the treasure, and then get back to
the portal before the timer runs out!
The timer starts as soon as you grab the treasure.

![screenshot](images/screenshot.png)


## Generating Documentation

```
cd /home/workspace/symmetrical-garbanzo
apt install doxygen graphviz
doxygen doxygen_settings.cfg
```

## File and Class Structure

For details on all classes, please generate documentation using doxygen and point your web browser to `docs/html/index.html`. Here I give an overview.

The main game loop is in Game::run(), which can be found in src/Game.cpp.

All the window creation and opengl-related stuff can be found in the GraphicsWindow class and the Shader class.

The game objects are all instances of `Entity`, but the architecture is mainly using old-fashioned inheritance rather than Entity-Component-System.

Entities can be drawn, and they can be updated. The job of the game loop is to handle input, update all entities, and then render all entities.


## Rubric points addressed

- The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
  - [src/Shader.cpp:8](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/Shader.cpp#L8) reads shaders from a file to support the class `Shader`, which compiles and links the shaders for opengl rendering.
  - [src/StbImage.cpp:11](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/StbImage.cpp#L11) uses the third party library stb_image to load image data into memory for use as textures.
- The project accepts input from a user as part of the necessary operation of the program.
  - [src/Game.cpp:182](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/Game.cpp#L182) and [src/GameWindow.cpp:66](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/GameWindow.cpp#L66) handle user controls.
- The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
  - Every header file in the `include/` directory contains at least one class or struct definition.
- All class members that are set to argument values are initialized through member initialization lists.
  - For example [src/GraphicalEntity.cpp:6](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/GraphicalEntity.cpp#L6) or [src/GameWindow.cpp:235](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/src/GameWindow.cpp#L235).
- Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
  - [include/Hud.h:70](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/Hud.h#L70). The class constructor only takes a width, and it determines a height based on other constraints. The width and height are private.
  - [include/Camera.h:6](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/Camera.h#L6). The `Camera` class is a good example where an invariant needs to be maintained: the `dir` and `right` members need to remain orthonormal and `right` needs to maintain a certain relationship with `dir` and `world_up`. This is so that (a) when the player strafes to the right we know which way that is and (b) when the camera is made to rotate by player action we are able to generate a view transformation matrix to use for rendering. You can see getter functions in [include/Camera.h:27](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/Camera.h#L27)
- One function is overloaded with different signatures for the same function name.
  - [include/Shader.h:35](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/Shader.h#L35)
- One member function in an inherited class overrides a virtual base class member function.
  - Each Entity in the game does this at least for the `update` and `draw` member functions.
  - Another example is [include/GraphicalEntity.h:29](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/GraphicalEntity.h#L29)
- One function is declared with a template that allows it to accept a generic parameter.
  - [include/GameWindow.h:120](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/GameWindow.h#L120)
- At least two variables are defined as references, or two functions use pass-by-reference in the project code.
  - [include/GameWindow.h:168](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/GameWindow.h#L168)
  - [include/GameWindow.h:112](https://github.com/ebrahimebrahim/symmetrical-garbanzo/blob/c1f63074296a6987f10be3fa9948d1edea124ebe/include/GameWindow.h#L112)

# symmetrical-garbanzo

This is my final project for the Udacity C++ course.
This branch of the project is specifically made to run in the Udacity workspace,
which has some limitations.
Look at the main branch for the actual game.

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
| look              | mouse movement |
| restart           | R              |
| toggle fullscreen | F              |

You will spawn in front of a portal in a randomly generated level.
The level has a treasure in it. Find the treasue, and then get back to
the portal before the timer runs out!
The timer starts as soon as you grab the treasure.

![screenshot](images/screenshot.png)


## Generating Documentation

```
cd /home/workspace/symmetrical-garbanzo
apt install doxygen
doxygen doxygen_settings.cfg
```

## File and Class Structure

For details on all classes, please generate documentation using doxygen and point your web browser to `docs/html/index.html`. Here I give an overview.

The main game loop is in Game::run(), which can be found in src/Game.cpp.

All the window creation and opengl-related stuff can be found in the GraphicsWindow class and the Shader class.

The game objects are all instances of `Entity`, but the architecture is mainly using old-fashioned inheritance rather than Entity-Component-System.

Entities can be drawn, and they can be updated. The job of the game loop is to handle input, update all entities, and then render all entities.


## Rubric points addressed



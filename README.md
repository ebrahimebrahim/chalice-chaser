# symmetrical-garbanzo

## Building

Install opengl dependencies on linux:
```
sudo apt-get install libgl1-mesa-dev libglfw3-dev
```

Build project on linux:
```
mkdir build
cd build
cmake ..
make
```

Generate documentation:
```
doxygen doxygen_settings.cfg
```

## Playing

Execute `run.sh`.

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

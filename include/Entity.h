#ifndef ENTITY_H
#define ENTITY_H

class GameWindow;

class Entity {
    static int next_id;
    int id;
    GameWindow * game_window; /** handle to game window, non-owned */
    virtual ~Entity() = 0;
public:
    Entity(GameWindow * game_window);
    int get_id() const {return id;}
    virtual void update() = 0;
    virtual void draw() = 0;
};

#endif // ENTITY_H
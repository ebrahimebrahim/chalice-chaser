#ifndef ENTITY_H
#define ENTITY_H

class Entity {
    static int next_id;
    int id;

public:
    Entity() {id = next_id++;}
    virtual ~Entity() = default;
    int get_id() const {return id;}
    virtual void update(double delta) = 0; /** delta is the time step of the update in seconds */
    virtual void draw() const = 0;
};

#endif // ENTITY_H
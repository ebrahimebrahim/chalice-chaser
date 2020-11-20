#include<Entity.h>
#include <glm/glm.hpp>

#ifndef PRIZE_H
#define PRIZE_H

class Prize : public Entity {
public:
    glm::mat4 model_matrix;
    Prize(GameWindow * game_window);
    void update(double delta) override;
    void draw() override;
};

#endif
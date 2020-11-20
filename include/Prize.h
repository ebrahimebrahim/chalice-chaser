#ifndef PRIZE_H
#define PRIZE_H

#include<Entity.h>
#include <glm/glm.hpp>


class Prize : public Entity {
public:
    glm::mat4 model_matrix;
    Prize(GameWindow * game_window);
    void update(double delta) override;
    void draw() const override;
};

#endif // PRIZE_H
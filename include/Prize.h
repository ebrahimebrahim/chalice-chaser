#ifndef PRIZE_H
#define PRIZE_H

#include<Entity.h>
#include <glm/glm.hpp>


class Prize : public Entity {
    float rot{}; /** angle in radians about vertical axis */
    glm::mat4 model_matrix;
public:
    glm::vec3 pos{0.0f, 0.0f, 0.0f};
    auto get_model_matrix() const {return model_matrix;}

    Prize(GameWindow * game_window);
    void update(double delta) override;
    void draw() const override;
};

#endif // PRIZE_H
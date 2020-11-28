#ifndef PORTAL_H
#define PORTAL_H

#include<GraphicalEntity.h>


class Portal : public GraphicalEntity {
    float phase{}; /**< used for animation in shader */
public:
    Portal(GameWindow * game_window);

    static const float height;

    GraphicsData create_graphics_data() override;
    void update(double delta) override;
    void draw(glm::vec3 player_position) const override;
};

#endif // PORTAL_H
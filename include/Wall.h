#ifndef WALL_H
#define WALL_H

#include<GraphicalEntity.h>


class Wall : public GraphicalEntity {
public:
    Wall(GameWindow * game_window);

    static const float wall_height;

    GraphicsData create_graphics_data() override;
    void update(double delta) override;
};

#endif // WALL_H
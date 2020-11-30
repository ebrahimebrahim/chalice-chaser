#ifndef WALL_H
#define WALL_H

#include<GraphicalEntity.h>


class Wall : public GraphicalEntity {
public:
    Wall(GameWindow * game_window);

    static const float wall_height;

    GraphicsData create_graphics_data() override;
    void update(double delta) override;

    /** rotate the wall about the vertical axis centered on the center of the block that the wall is the back face of*/
    void rotate_about_block_center(float angle);
};

#endif // WALL_H
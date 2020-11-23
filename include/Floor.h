#ifndef Floor_H
#define Floor_H

#include<GraphicalEntity.h>


class Floor : public GraphicalEntity {
    float xdim;
    float zdim;
public:
    Floor(GameWindow * game_window, float xdim, float zdim);

    GraphicsData create_graphics_data() override;
    void update(double delta) override;
};

#endif // Floor_H
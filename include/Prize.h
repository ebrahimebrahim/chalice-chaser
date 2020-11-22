#ifndef PRIZE_H
#define PRIZE_H

#include<GraphicalEntity.h>


class Prize : public GraphicalEntity {
public:
    Prize(GameWindow * game_window);

    GraphicsData create_graphics_data() override;
    void update(double delta) override;
};

#endif // PRIZE_H
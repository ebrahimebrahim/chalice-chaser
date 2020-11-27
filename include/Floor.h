#ifndef Floor_H
#define Floor_H

#include<GraphicalEntity.h>
#include <string>

class Floor : public GraphicalEntity {
    float xdim;
    float zdim;
    std::string texture_filename;
public:
    Floor(GameWindow * game_window, const char * texture_filename, float xdim, float zdim);

    GraphicsData create_graphics_data() override;
    void update(double delta) override;
};

#endif // Floor_H
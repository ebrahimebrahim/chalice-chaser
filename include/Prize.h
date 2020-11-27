#ifndef PRIZE_H
#define PRIZE_H

#include<GraphicalEntity.h>


class Prize : public GraphicalEntity {
    static std::optional<GraphicsData> graphics_data;

public:
    Prize(GameWindow * game_window);

    GraphicsData create_graphics_data() override;
    void update(double delta) override;

    /**Prize::create_graphics_data is different from the usual GraphicalEntity.
     * The static variable `Prize::graphics_data` is created once by a function that generates it, when `get_graphics_data` is called the first time.
     * After it's created, `get_graphics_data` will return it every time.
     * This way the data becomes accessible to anyone who can see the `Prize` class
     * */ 
    static GraphicsData get_graphics_data();
    static void reset_graphics_data();
    
    static const std::vector<glm::vec3> star_colors;
    static const std::vector<glm::vec3> gem_colors;
};

#endif // PRIZE_H
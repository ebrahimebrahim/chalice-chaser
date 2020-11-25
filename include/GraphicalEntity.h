#ifndef GRAPHICAL_ENTITY_H
#define GRAPHICAL_ENTITY_H

#include<Entity.h>
#include <glm/glm.hpp>

const float TAU = 6.2831853f;

class GameWindow;
struct GraphicsData;

/**An abstract Entity that implements draw() in a way that interfaces with a GameWindow.
 * If you inherit from this then you *must* define a constructor that calls add_self_to_game_window. Something like this:
 * `DerivedGraphicalEntity(GameWindow * game_window) : GraphicalEntity(game_window) { add_self_to_game_window(); }`
 * Override create_graphics_data to return a struct that specifies how to draw your object.
 * Then GraphicalEntity will take care of removing the object from the game window on destruction.
 * Duplication is done smartly in the copy constructor, sharing graphics data in video memory among duplicates.
 * Moving also works.
*/
class GraphicalEntity : public Entity {
    glm::mat4 model_matrix;
    glm::mat4 model_matrix_without_translation;
    void update_model_matrix(); // Update the model matrix by combining `pos` and `model_matrix_without_translation` and carrying out the translation
protected:
    GameWindow * game_window; /** handle to game window, non-owned */
    void add_self_to_game_window();
public:
    auto get_model_matrix() const {return model_matrix;}
    void set_pos(const glm::vec3 new_pos) override;
    void rotate(float angle, glm::vec3 axis);

    GraphicalEntity(GameWindow * game_window);
    GraphicalEntity(const GraphicalEntity &);
    GraphicalEntity(GraphicalEntity &&);
    GraphicalEntity & operator=(const GraphicalEntity &);
    GraphicalEntity & operator=(GraphicalEntity &&);
    ~GraphicalEntity();

    virtual GraphicsData create_graphics_data() = 0;
    
    virtual void draw(glm::vec3 player_position) const override;
};

#endif // GRAPHICAL_ENTITY_H
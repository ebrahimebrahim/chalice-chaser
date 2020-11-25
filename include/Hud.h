#ifndef HUD_H
#define HUD_H

#include<Entity.h>
#include <glm/glm.hpp>

class Player;
class GameWindow;

class Hud : public Entity {
    GameWindow * game_window; /** handle to game window, non-owned */
protected:
public:
    Hud(GameWindow * game_window, const Player * player);
    ~Hud();

    void draw(glm::vec3 player_position) const override;
    void update(double delta) override;

    const Player * player;

};

#endif // HUD_H
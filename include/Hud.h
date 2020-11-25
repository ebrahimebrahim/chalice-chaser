#ifndef HUD_H
#define HUD_H

#include<Entity.h>
#include <glm/glm.hpp>
#include <memory>

class Player;
class GameWindow;
class PrizeHud;


class Hud {
    GameWindow * game_window; /** handle to game window, non-owned */
    std::unique_ptr<PrizeHud> prize_hud;
    bool player_has_prize{false};
public:
    Hud(GameWindow * game_window, const Player * player) : game_window{game_window}, player(player) {}

    const Player * player; /** handle to player, non-owned */

    void update();
    void draw() const;
    
    
};


class PrizeHud : public Entity {
    GameWindow * game_window; /** handle to game window, non-owned */
protected:
public:
    PrizeHud(GameWindow * game_window);
    ~PrizeHud();

    void draw(glm::vec3 player_position) const override;
    void update(double delta) override;
};

#endif // HUD_H
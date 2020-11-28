#ifndef HUD_H
#define HUD_H

#include <Entity.h>
#include <glm/glm.hpp>
#include <memory>

class GameWindow;
class Player;
class Timer;
class PrizeHud;
class TimerHud;
class MessageHud;


class Hud {
    GameWindow * game_window; /**< handle to game window, non-owned */
    std::unique_ptr<PrizeHud> prize_hud;
    std::unique_ptr<TimerHud> timer_hud;
    std::unique_ptr<MessageHud> message_hud;
    bool player_has_prize{false};
public:
    Hud(GameWindow * game_window, const Player * player, const Timer * timer) : game_window{game_window}, player{player}, timer{timer} {}

    // non-owned handles
    const Player * player;
    const Timer * timer;

    void update();
    void draw() const;

    void show_message(const char * texture_filename);
    
};


class PrizeHud : public Entity {
    GameWindow * game_window; /**< handle to game window, non-owned */
public:
    PrizeHud(GameWindow * game_window);
    ~PrizeHud(); // TODO: implement/delete rest of the big 5

    void draw(glm::vec3 player_position) const override;
    void update(double delta) override {}
    void update(); // Not an override!
};

class TimerHud : public Entity {
    GameWindow * game_window; /**< handle to game window, non-owned */
    glm::mat4 scale{1.0f}; // gets updated in `update`
    glm::mat4 translate{0.0f}; // bogus matrix, it's actually initialized in ctor
    float fraction; // the current fraction of this progress bar, from 0.0 to 1.0

public:
    TimerHud(GameWindow * game_window);
    ~TimerHud(); // TODO: implement/delete rest of the big 5

    void draw(glm::vec3 player_position) const override;
    void update(double delta) override {}
    void update(); // Not an override!

    float max_width{0.3}; /**< In normal window coords, -1 to 1 being from left to right edge of window */
    float height{0.1}; /**< In normal window coords */
    glm::vec2 botleft_pos{0.6,0.8}; /**< In normal window coords, location of bottom left corner */

    void set_fraction(float frac) {fraction=frac;}
};

/** A display of a texture in the middle of the screen. */
class MessageHud : public Entity {
    GameWindow * game_window; /**< handle to game window, non-owned */
    float width{};
    float height{};

public:
    /** The width is given in screen normal coordinates, which run from -1.0 to 1.0.
     *  Height will then be set based on the texture image aspect ratio and the screen aspect ratio.
     */
    MessageHud(GameWindow * game_window, const char * texture_filename, float width);
    ~MessageHud(); // TODO: implement/delete rest of the big 5

    void draw(glm::vec3 player_position) const override;
    void update(double delta) override {}
};

#endif // HUD_H
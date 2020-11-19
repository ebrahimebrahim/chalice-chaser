#include <Entity.h>

int Entity::next_id = 0;

Entity::Entity(GameWindow * game_window) : game_window{game_window} {
    id = next_id++;
}
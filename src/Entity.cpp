#include <Entity.h>

int Entity::next_id = 0;

Entity::Entity(){
    id = next_id++;
}
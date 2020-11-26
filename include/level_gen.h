#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

#include <glm/glm.hpp>
#include <vector>

namespace LevelGen{

const int TILEMAP_SIZE = 10;
typedef glm::ivec2 vec;

/** "oriented line of tiles"
 *  Imagine this as a line of tiles starting at x and extending m tiles in the d direction,
 *  with the vector w pointing in the direction we want to "mine".
 *  So the tiles included are x, x+d, x+2d, ..., x+(m-1)d
 */
struct Olt { 
    vec x;
    vec d;
    int m;
    vec w;
};


/**Represents a generated level in the abstract as tiles with properites.
 * The tiles are accessed via `get_tile` by putting in LevelGen::vec locations, which are glm::vec2
 * The tiles are just bool, which indicates whether or not the tile is passable.
 * So `true` is for floor and `false` is for wall.
 * Additionally there is a treasure, whose location is the member variable `treasure_location`.
 * And there are start tiles to indicate the entrance/exit. Use `is_start` to test for those.
 * 
*/
struct Tilemap {

    Tilemap() : treasure_location(-1,-1) {}

    void print() const;

    char tile_to_char(const vec & loc) const;

    void validate_tile(const vec & location) const;

    bool get_tile(const vec & location) const { validate_tile(location); return tiles[location[1]][location[0]]; }
    void set_tile(const vec & location, bool val) { validate_tile(location); tiles[location[1]][location[0]] = val; }
    
    void set_start(const Olt & olt);
    bool is_start(const vec & location) const;
    vec get_player_spawn_location() const {return middle_of_start_area;}
    
    bool is_treasure(const vec & location) const {return treasure_location==location;}

    vec treasure_location;

private:
    bool tiles[TILEMAP_SIZE][TILEMAP_SIZE]{};
    std::vector<vec> start_area;
    vec middle_of_start_area{0,0};
};

/** Generate a tilemap.
 *  A lot of numbers in the functon definition are hardcoded but could easily be made into parameters of this function if I later want.
 *  See function definition for more details. */
Tilemap generate_level();



} // namespace LevelGen

#endif // LEVEL_GEN_H
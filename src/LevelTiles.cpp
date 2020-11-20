// This will start as an experimental program with a main on its own,
// then the class definitions and function declarations will be moved to a header,
// main will be deleted, and the class will be part of the game program.

#include <iostream>
#include <bitset>
#include <unordered_map>
#include <random>
#include <chrono>
#include <vector>
#include <glm/glm.hpp>

enum TileBits {TB_WALL_NORTH = 0, TB_WALL_SOUTH, TB_WALL_EAST, TB_WALL_WEST, TB_TREASURE, TB_EXIT, NUM_TILEBITS};
typedef std::bitset<NUM_TILEBITS> Tile;
typedef glm::uvec2 location;


constexpr unsigned TILEMAP_SIZE = 30;

std::unordered_map<Tile,char> tile_to_char = {
    {0b0000, '.'},
    {0b0001, '^'},
    {0b0010, 'v'},
    {0b0011, '|'},
    {0b0100, '>'},
    {0b0101, '7'},
    {0b0110, '4'},
    {0b0111, ']'},
    {0b1000, '<'},
    {0b1001, 'P'},
    {0b1010, '2'},
    {0b1011, '['},
    {0b1100, '-'},
    {0b1101, 'A'},
    {0b1110, 'U'},
    {0b1111, 'O'},
};

struct TileMap {
    Tile tiles[TILEMAP_SIZE][TILEMAP_SIZE]{};

    void print() const {
        for (int i=0; i < TILEMAP_SIZE; ++i){
            for (int j=0; j < TILEMAP_SIZE; ++j){
                if (tiles[i][j][TB_TREASURE])
                    std::cout << 'X';
                else if (tiles[i][j][TB_EXIT])
                    std::cout << '~';
                else
                    std::cout << tile_to_char[tiles[i][j]];
            }
            std::cout << '\n';
        }
    }
};


std::vector<std::pair<unsigned,unsigned>> random_intervals(
    int a, int b,
    unsigned min_intervals, unsigned max_intervals,
    unsigned min_width, unsigned max_width)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rand(seed);

    

}


int main() {
    TileMap tm{};

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rand(seed);
    
    


    tm.print();
}
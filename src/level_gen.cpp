#include <level_gen.h>

#include <iostream>
#include <random>
#include <chrono>
#include <bitset>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <stdexcept>

namespace LevelGen{



bool within_grid(const vec & location) {
    return
        0 <= location[0] &&
        location[0] < TILEMAP_SIZE &&
        0 <= location[1] &&
        location[1] < TILEMAP_SIZE;
}



void Tilemap::print() const {
    for (int i=0; i < TILEMAP_SIZE; ++i){
        for (int j=0; j<TILEMAP_SIZE; ++j) {
            std::cout << tile_to_char(vec(j,i));
        }
        std::cout << '\n';
    }
}

void Tilemap::validate_tile(const vec & location) const { 
    if (!within_grid(location))
        throw std::range_error("Invalid tilemap access!");
}

char Tilemap::tile_to_char(const vec & loc) const {
    bool open_space = get_tile(loc);
    if (is_start(loc)){
        if (is_treasure(loc))
            return 'X'; // represents an error, shouldn't happen!
        else if (loc==get_player_spawn_location())
            return (open_space ? 'P' : 'X');
        else
            return (open_space ? '~' : 'X');
    }
    else if (is_treasure(loc))
        return (open_space ? 'O' : 'X');
    else    
        return (open_space ? '.' : '|');
}

void Tilemap::set_start(const Olt & olt) {
    for (int i=0; i<olt.m; ++i)
        start_area.push_back(olt.x+i*olt.d);
    middle_of_start_area = olt.x + (olt.m/2) * olt.d;
}

bool Tilemap::is_start(const vec & location) const {
    return std::any_of(start_area.begin(), start_area.end(), [location](const vec & start_loc){return start_loc == location;});
}


/** The idea is to take an olt representing a long wall, and then
 *  to subdivide it into a bunch of olt's that represent openings to be created.
 *  We will attempt to create n_min to n_max openings of size l_min to l_max.
 *  No guarantees though.
 */
std::vector<Olt> subdivide(const Olt & olt, int n_min, int n_max, int l_min, int l_max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> n_chooser(n_min, n_max);
    int n = n_chooser(gen);

    std::uniform_int_distribution<int> index_chooser(0, olt.m-1);
    std::vector<int> indices;
    for (int i = 0; i < n; ++i) {
        bool give_up = false;
        for (int attempts = 0; true; ++attempts) {
            int k = index_chooser(gen);
            if (std::all_of(indices.begin(), indices.end(),[k](int chosen_k){ return (glm::abs(k-chosen_k) > 1); })) {
                indices.push_back(k);
                break;
            }
            if (attempts > 200) {
                give_up = true;
                break;
            }
        }
        if (give_up) break;
    }

    std::vector<Olt> new_olts;
    for (auto k : indices) {
        vec x = olt.x + k*olt.d;
        std::uniform_int_distribution<int> m_chooser(l_min, l_max);
        int max_m = m_chooser(gen);
        int m;
        for (m = 1; m < max_m; ++m) {
            vec two_ahead = x + (m+1)*olt.d;
            if (!within_grid(two_ahead))
                break;
            if (std::any_of( indices.begin() , indices.end() , [k,m](int index){return index == k+m+1;}))
                break;
            if (k+m==olt.m)
                break;
        }
        new_olts.push_back({x, olt.d, m, olt.w});
    }
    return new_olts;
}

/** Mine out up to a randomly chosen proporition of the max distance.
 *  Return the two side walls as Olt's,
 *  or return an empty vector if no mining took place because there is not
 *  much distance to mine.
 */
std::vector<Olt> mine(Tilemap & tm, const Olt & olt) {
    int j; // The max allowed distance to mine out in the olt.w direction
    for (j = 0; true; ++j) {
        if (!within_grid(olt.x+j*olt.w))
            break;
        bool all_walls = true;
        for (int i=0; i<olt.m; ++i) {
            if ( tm.get_tile(olt.x+j*olt.w+i*olt.d) )
                all_walls = false;
        }
        if (!all_walls)
            break;

        if (within_grid(olt.x+j*olt.w + (-1)*olt.d))
            if (tm.get_tile(olt.x+j*olt.w + (-1)*olt.d))
                break;
        if (within_grid(olt.x+j*olt.w + olt.m*olt.d))
            if (tm.get_tile(olt.x+j*olt.w + olt.m*olt.d))
                break;
    }
    

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> proportion_chooser(0.5, 1.0);
    float proportion = proportion_chooser(gen);
    int mine_distance = glm::round(proportion * float(j));

    // if we won't mine out very far then just don't even do it.
    if (mine_distance<5) return std::vector<Olt>{};

    for (int i=0; i<olt.m; ++i){
        for (int j=0; j<mine_distance; ++j)
            tm.set_tile(olt.x+j*olt.w+i*olt.d,true);
    }

    std::vector<Olt> new_olts;
    if (within_grid(olt.x - olt.d))
        new_olts.push_back({olt.x - olt.d, olt.w, mine_distance, -olt.d});
    if (within_grid(olt.x + olt.m*olt.d))
        new_olts.push_back({olt.x + olt.m*olt.d, olt.w, mine_distance, olt.d});
    
    return new_olts;
}

/** Generate a tilemap.
 *  A lot of numbers in the functon definition are hardcoded but could easily be made into parameters of this function if I later want.
 *  They are:
 *      m_min, m_max (the range of sizes of the level entrance/exit)
 *      and the parameters that get passed into subdivide
 */
Tilemap generate_level() {
    Tilemap tm{};

    int m_min = 3;
    int m_max = 8;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> x1_chooser(0, TILEMAP_SIZE-m_min);
    int x1 = x1_chooser(gen);
    std::uniform_int_distribution<int> m_chooser(m_min,m_max);
    int m = std::min(m_chooser(gen), TILEMAP_SIZE-x1);

    std::deque<Olt> s;
    s.push_back({vec(x1,0), vec(1,0), m, vec(0,1)});
    tm.set_start(s.back());

    // keep trck of the iteration at which each olt in the loop below was (possibly) mined out
    // will give a rough estimate of how 'deep' different tiles are, to help choose treasure loc
    std::vector<std::pair<Olt,int>> olts_and_iter;
    int iteration = 0;

    while (!s.empty()) {
        Olt olt;
        if (gen()%2){
            olt = s.front();
            s.pop_front();
        }
        else{
            olt =s.back();
            s.pop_back();
        }
        olts_and_iter.emplace_back(olt,iteration);

        auto wall_olts = mine(tm, olt);
        for (auto & wall_olt : wall_olts){
            auto olts = subdivide(wall_olt,1,std::max(1,wall_olt.m/8),2,5);
            for (auto & o : olts) s.push_back(o);
        }
        ++iteration;
    }

    std::sort(olts_and_iter.begin(), olts_and_iter.end(),
        [](std::pair<Olt,int> p1, std::pair<Olt,int> p2){return p1.second > p2.second;}
    );

    // Choose an olt as pointing (via its w) to the corrdior in which to place a teasure
    // Do this by going through all the olts that might have been mined out, starting with the "deepest" in the iteration
    // Since we randomly "pop_back" and "pop_front" in the loop above, this ordering is pretty rough, which is good for variation.
    Olt olt_treasure; 
    for (const auto & p : olts_and_iter) {
        Olt olt = p.first;
        bool all_open = true;
        for (int i =0; i<olt.m; ++i){
            if (! tm.get_tile(olt.x+i*olt.d)) {
                all_open = false;
                break;
            }
        }
        if (all_open) {
            olt_treasure = olt;
            break;
        }
    }

    int mine_depth{};
    for (int j = 0; true; ++j) {

        if (!within_grid(vec(olt_treasure.x + j*olt_treasure.w))){
            mine_depth = j;
            break;
        }

        bool all_open = true;
        for (int i =0; i<olt_treasure.m; ++i){
            if (! tm.get_tile(olt_treasure.x + j*olt_treasure.w + i*olt_treasure.d)) {
                all_open = false;
                break;
            }
        }
        if (!all_open) {
            mine_depth = j;
            break;
        }
    }


    // Now we have a corrdior in which to place a treasure.
    // In terms of olt_treasure's member vars, the locations are
    // x + i*d + j*w running over all i in [0,m) and all j in [0,mine_depth)
    int i = gen()%olt_treasure.m;
    int j = gen()%mine_depth;
    tm.treasure_location = olt_treasure.x + i*olt_treasure.d + j*olt_treasure.w;

    return tm;
}

} // namespace LevelGen
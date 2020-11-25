#include <Prize.h>
#include <GameWindow.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/trigonometric.hpp>

#include <random>
#include <chrono>

std::optional<GraphicsData> Prize::graphics_data{}; // start as std::null_opt

typedef std::uniform_real_distribution<float> float_dist;
typedef std::uniform_int_distribution<int> int_dist;


/** Fills the contents of vertices and indices. */
void make_star_mesh(std::vector<GLfloat> & vertices, std::vector<GLuint> & indices) {
    vertices.clear(); // in case they weren't clear for some reason
    indices.clear();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    const float radius = float_dist(0.3,0.5)(gen);
    float inset_radius;
    if (gen()%3) {
        inset_radius = float_dist(0.1, radius)(gen);
    }
    else
        inset_radius = radius;
    const float thickness = float_dist(0.05,0.3)(gen);
    const int n_segs = 2 * int_dist(2,20)(gen);
    const float phase = float_dist(0.0,TAU)(gen);

    const float front_face_z = thickness/2.0;
    const float back_face_z = -front_face_z;
    vertices.insert(vertices.end(), {0,0,front_face_z});
    const unsigned i_front_face_center = (vertices.size() - 1)/3;
    vertices.insert(vertices.end(), {0,0,back_face_z});
    const unsigned i_back_face_center = (vertices.size() - 1)/3;
    const unsigned i_front_face_first = vertices.size()/3;
    for (int i = 0 ; i < n_segs ; ++i) {
        const float theta = float(i)*TAU / float(n_segs) + phase;
        const float r = (i%2)? radius : inset_radius;
        vertices.insert(vertices.end(), {r*glm::cos(theta), r*glm::sin(theta), front_face_z});
    }
    const unsigned i_back_face_first = vertices.size()/3;
    for (int i = 0 ; i < n_segs ; ++i) {
        const float theta = float(i)*TAU / float(n_segs) + phase;
        const float r = (i%2)? radius : inset_radius;
        vertices.insert(vertices.end(), {r*glm::cos(theta), r*glm::sin(theta), back_face_z});
    }

    for (int i = 0 ; i < n_segs ; ++i) {
        indices.insert(indices.end(), {i_front_face_center, i_front_face_first + i, i_front_face_first + (i + 1)%n_segs});
        indices.insert(indices.end(), {i_back_face_center, i_back_face_first + i, i_back_face_first + (i + 1)%n_segs});
        indices.insert(indices.end(), {i_front_face_first + i, i_front_face_first + (i + 1)%n_segs, i_back_face_first + (i + 1)%n_segs});
        indices.insert(indices.end(), {i_front_face_first + i, i_back_face_first + (i + 1)%n_segs, i_back_face_first + i});
    }
}



static GraphicsData generate_prize_graphics_data() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    GraphicsData d;

    switch(gen()%3) {
        case 0: 
            make_star_mesh(d.vertices, d.indices);
            break;
        
        case 1: // chalices
            d.vertices = {
                0.0f, 0.0f, 0.0f , // center of fan
                -0.3f, -0.3f, -0.3f ,
                0.3f, -0.3f, -0.3f ,
                0.3f, -0.3f, 0.3f ,
                -0.3f, -0.3f, 0.3f ,
            };
            d.indices = { 0,1,2,3,4 };
            break;
        case 2: // gems
            d.vertices = {
                0.0f, 0.0f, 0.0f , // center of fan
                -0.3f, -0.3f, -0.3f ,
                0.3f, -0.3f, -0.3f ,
                0.3f, -0.3f, 0.3f ,
                -0.3f, -0.3f, 0.3f ,
            };
            d.indices = { 0,1,2,3,4 };
            break;
    }

    d.draw_mode = GL_TRIANGLES;
    d.shader_choice = ShaderChoice::SHADER_DEFAULT;
    
    return d;
}




Prize::Prize(GameWindow * game_window) : GraphicalEntity(game_window) {
    add_self_to_game_window();
    collision_box = CollisionBox(
        glm::vec3{0.0f, 0.0f, 0.0f}, // center
        glm::vec3{0.8f, 1.0f, 0.8f}  // extents
    );
}

GraphicsData Prize::create_graphics_data() {
    return get_graphics_data();
}

GraphicsData Prize::get_graphics_data() {
    if (!graphics_data)
        graphics_data = generate_prize_graphics_data();
    return *graphics_data;
}

void Prize::update(double delta) {
    rotate((TAU/3.0f) * delta, glm::vec3(0.0f,1.0f,0.0f));
}
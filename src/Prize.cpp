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
    int n_segs;
    if (gen()%3) {
        inset_radius = float_dist(0.1, radius)(gen);
        n_segs = 2 * int_dist(2,10)(gen);
    }
    else {
        inset_radius = radius;
        n_segs = 2 * int_dist(2,32)(gen);
    }
    const float thickness = float_dist(0.05,0.2)(gen);
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


void make_chalice_mesh(std::vector<GLfloat> & vertices, std::vector<GLuint> & indices) {
    vertices.clear(); // in case they weren't clear for some reason
    indices.clear();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // (1) make a spline
    
    std::vector<std::pair<float,float>> spline; // r and z pairs (think cylindrical coordinates)

    // (1a) chalice is made of cup, handle, and base. this is cup.

    spline.push_back({ float_dist(0.15,0.35)(gen) , float_dist(0.4,0.6)(gen) }); // cup top
    spline.push_back({ float_dist(0.2,0.45)(gen) , float_dist(0.2,0.4)(gen) }); // cup mid
    spline.push_back({ float_dist(0.15,0.25)(gen) , 0.0 }); // cup bottom

    // (1b) handle
    const int n_handle_segs = int_dist(3,10)(gen);
    for (int i=0;i<n_handle_segs;++i) {
        spline.push_back( {  float_dist(0.05,0.08)(gen) , float(-i)/float(n_handle_segs) * 0.5  } );
    }


    // (1c) base

    spline.push_back( { float_dist(0.4,0.45)(gen) , float_dist(-0.6,-0.5)(gen) } );
    

    // (2) make mesh using spline, in "lathe" style

    const unsigned segs = 10;

    for (const auto & ry : spline) {
        const float r = ry.first;
        const float y = ry.second;
        for (int j = 0 ; j < segs ; ++j) {
            const float theta = float(j)*TAU / float(segs);
            vertices.insert(vertices.end(), {r*glm::cos(theta), y, r*glm::sin(theta)});
        }
    }

    for (int i=0; i<spline.size()-1; ++i) {
        const unsigned ring_start_index = i*segs;
        const unsigned next_ring_start_index = (i+1)*segs;
        for (int j = 0 ; j < segs ; ++j) {
            indices.insert(indices.end(), {ring_start_index + j, next_ring_start_index + j, ring_start_index + (j+1)%segs} );
            indices.insert(indices.end(), {next_ring_start_index + j, next_ring_start_index + (j+1)%segs, ring_start_index + (j+1)%segs} );
        }
    }

}


void make_gem_mesh(std::vector<GLfloat> & vertices, std::vector<GLuint> & indices) {
    vertices.clear(); // in case they weren't clear for some reason
    indices.clear();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // (1) make a spline
    
    std::vector<std::pair<float,float>> spline; // r and z pairs (think cylindrical coordinates)

    const float y1 = float_dist(0.3,0.5)(gen);
    const float y2 = float_dist(0.1,0.2)(gen);
    const float r2 = float_dist(0.2,0.6)(gen);
    const float r1 = float_dist(0.1,r2)(gen);

    spline.push_back({ 0.0 , y1 });
    spline.push_back({ r1 , y2 });
    spline.push_back({ r2 , 0.0 });
    spline.push_back({ r1 , -y2 });
    spline.push_back({ 0.0 , -y1 });

    // (2) make mesh using spline, in "lathe" style

    const unsigned segs = int_dist(3,6)(gen);

    for (const auto & ry : spline) {
        const float r = ry.first;
        const float y = ry.second;
        for (int j = 0 ; j < segs ; ++j) {
            const float theta = float(j)*TAU / float(segs);
            vertices.insert(vertices.end(), {r*glm::cos(theta), y, r*glm::sin(theta)});
        }
    }

    for (int i=0; i<spline.size()-1; ++i) {
        const unsigned ring_start_index = i*segs;
        const unsigned next_ring_start_index = (i+1)*segs;
        for (int j = 0 ; j < segs ; ++j) {
            indices.insert(indices.end(), {ring_start_index + j, next_ring_start_index + j, ring_start_index + (j+1)%segs} );
            indices.insert(indices.end(), {next_ring_start_index + j, next_ring_start_index + (j+1)%segs, ring_start_index + (j+1)%segs} );
        }
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
            make_chalice_mesh(d.vertices, d.indices);
            break;
        case 2: // gems
            make_gem_mesh(d.vertices, d.indices);
            break;
    }

    d.draw_mode = GL_TRIANGLES;
    d.shader_choice = ShaderChoice::SHADER_DEFAULT;
    d.object_color = glm::vec3(0.702,0.686,0.267);
    
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

void Prize::reset_graphics_data() {
    graphics_data = std::nullopt;
}

void Prize::update(double delta) {
    rotate((TAU/3.0f) * delta, glm::vec3(0.0f,1.0f,0.0f));
}
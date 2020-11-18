#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <vector>
#include <string>
#include <stdexcept>
using namespace std::literals::string_literals;

class Shader {
    public:

        //The shader program's ID in OpenGL
        GLuint shader_program_id = 0;

        // Throws runtime errors when files not found or there are compiling/linking issues
        Shader(std::string vertex_shader_path, std::string fragment_shader_path);
        Shader() = default;
        Shader(const Shader&) = delete; // Do not copy. It manages a global OpenGL resource.
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&);
        Shader& operator=(Shader&&);
        ~Shader();


        // Tell OpenGL that we are now using this shader
        void use() const;
};



#endif // SHADER_H

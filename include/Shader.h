#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <stdexcept>
using namespace std::literals::string_literals;

class Shader {

        /** The shader program's ID in OpenGL */
        GLuint shader_program_id = 0;
    
    public:


        /** Throws runtime errors when files not found or there are compiling/linking issues */
        Shader(std::string vertex_shader_path, std::string fragment_shader_path);
        Shader() = default;
        Shader(const Shader&) = delete; // Do not copy. It manages a global OpenGL resource.
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&);
        Shader& operator=(Shader&&);
        ~Shader();


        /** Tell OpenGL that we are now using this shader */
        void use() const;

        /** Note: getting and setting uniform variables needs to be done AFTER glUseProgram on shader.
            So make sure to use "use" method above before doing this */
        void set_uniform(const char* name, const glm::mat4 & mat) const;
        void set_uniform(const char* name, const glm::vec3 & vec) const;
        void set_uniform(const char* name, float val) const;
        GLint get_uniform_location(const char* name) const;

};



#endif // SHADER_H

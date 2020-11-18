#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string fileToString(const char* filename){
  std::ifstream is(filename);
  if (!is.is_open()){
    std::string error_msg("Could not open file ");
    error_msg.append(filename);
    error_msg.append(".");
    throw std::runtime_error(error_msg);
  }
  std::stringstream ss;
  ss << is.rdbuf();
  return ss.str();
}


Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path) {
  //load and compile vertex shader
  std::string vertexShaderSource = fileToString(vertex_shader_path.c_str());
  const char* vertexShaderSource_cstring = vertexShaderSource.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (!vertexShader) throw std::runtime_error("OpenGL failed to create vertex shader.");
  glShaderSource(vertexShader,1,&vertexShaderSource_cstring,nullptr);
  glCompileShader(vertexShader);
  GLint compileSuccess;
  constexpr short infoLogSize = 512;
  char infoLog[infoLogSize];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
  if (compileSuccess == GL_FALSE){
    glGetShaderInfoLog(vertexShader,infoLogSize,nullptr,infoLog);
    std::string error_msg("Vertex shader compiler error:\n");
    error_msg.append(infoLog);
    throw std::runtime_error(error_msg);
  }

  //load and compile fragment shader
  std::string fragmentShaderSource = fileToString(fragment_shader_path.c_str());
  const char * fragmentShaderSource_cstring = fragmentShaderSource.c_str();
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!fragmentShader) throw std::runtime_error("OpenGL failed to create fragment shader.");
  glShaderSource(fragmentShader,1,&fragmentShaderSource_cstring,nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSuccess);
  if (compileSuccess == GL_FALSE){
    glGetShaderInfoLog(fragmentShader,infoLogSize,nullptr,infoLog);
    std::string error_msg("Fragment shader compiler error:\n");
    error_msg.append(infoLog);
    throw std::runtime_error(error_msg);
  }

  //link shaders
  shader_program_id = glCreateProgram();
  if (!shader_program_id) throw std::runtime_error("OpenGL failed to create a program object in which to link shaders.");
  glAttachShader(shader_program_id, vertexShader);
  glAttachShader(shader_program_id, fragmentShader);
  glLinkProgram(shader_program_id);
  GLint linkSuccess;
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &linkSuccess);
  if (linkSuccess == GL_FALSE){
    glGetProgramInfoLog(shader_program_id,infoLogSize,nullptr,infoLog);
    std::string error_msg("Shader program linking error:\n");
    error_msg.append(infoLog);
    throw std::runtime_error(error_msg);
  }

  //clean up, these are already linked in shader_program_id
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader(){
  if (shader_program_id)
    glDeleteProgram(shader_program_id);
  shader_program_id = 0;
}

// move constructor
Shader::Shader(Shader&& s) : shader_program_id{s.shader_program_id}
{
  s.shader_program_id = 0;
}

// move assignment operator
Shader& Shader::operator=(Shader&& s){
  if (shader_program_id)
    glDeleteProgram(shader_program_id);
  shader_program_id = s.shader_program_id;
  s.shader_program_id = 0;
  return *this;
}

void Shader::use() const {
  //use this shader_program_id for future drawing commands
  glUseProgram(shader_program_id);
}

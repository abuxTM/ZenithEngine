#include "ZenithEngine/core/shader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  // retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();		
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();		
  } catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // compile shaders
  GLuint vertex, fragment;
  int success;
  char infoLog[512];
     
  // vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  // print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  };
  
  fragment = glCreateShader(GL_FRAGMENT_SHADER);

  // Compile fragment shader
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
    
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

bool Shader::getBool(const std::string& name) const {
  GLint location = glGetUniformLocation(ID, name.c_str());

  GLint value;
  glGetUniformiv(ID, location, &value);
  return static_cast<bool>(value);
}

void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

float Shader::getFloat(const std::string& name) const {
  GLint location = glGetUniformLocation(ID, name.c_str());

  GLint value;
  glGetUniformiv(ID, location, &value);
  return static_cast<float>(value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

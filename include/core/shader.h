#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <glm/fwd.hpp>
#include <string>

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  void use();

  void setBool(const std::string& name, bool value) const;  
  bool getBool(const std::string& name) const;
  void setInt(const std::string& name, int value) const;   
  void setFloat(const std::string& name, float value) const;
  void setVec3(const std::string& name, glm::vec3 value) const;
  void setMat4(const std::string& name, const glm::mat4 &mat) const;

  GLuint ID;
};

#endif

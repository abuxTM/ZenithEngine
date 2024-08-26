#ifndef MESH_H
#define MESH_H

#include <glm/fwd.hpp>
#include "core/shader.h"
#include "core/window.h"
#include "glad/glad.h"

class Mesh {
public:
  Mesh(GLfloat* vertices, GLsizeiptr verticesSize);
  ~Mesh();

  void draw(Window& window, Shader& shader, glm::vec3 position, glm::vec3 scale, float rotation=0);

private:
  GLuint VBO, VAO;
};

#endif

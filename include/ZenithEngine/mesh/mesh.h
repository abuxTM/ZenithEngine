#ifndef MESH_H
#define MESH_H

#include <glm/fwd.hpp>
#include "ZenithEngine/core/camera.h"
#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/window.h"
#include "ZenithEngine/libs/glad/glad.h"

class Mesh {
public:
  Mesh(GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize, GLsizei stride);
  ~Mesh();

  void draw(Window& window, Shader& shader, glm::vec3 position=glm::vec3(0.f), glm::vec2 scale=glm::vec2(64.f), float rotation=0.f, glm::vec2 texRepeat=glm::vec2(1.f));
  void draw(Window& window, Camera& camera, Shader& shader, glm::vec3 position=glm::vec3(0.f), glm::vec3 scale=glm::vec3(4.f), glm::vec3 rotation=glm::vec3(0.f), glm::vec2 texRepeat=glm::vec2(1.f));
  void updateMesh(GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize);

  bool textured;
  glm::vec3 color;

private:
  GLuint VBO, VAO, EBO;
  bool useEBO;
};

#endif

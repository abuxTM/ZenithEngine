#include "mesh/mesh.h"
#include "core/shader.h"
#include "core/window.h"
#include <GL/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

Mesh::Mesh(GLfloat* vertices, GLsizeiptr verticesSize) {
  glGenBuffers(1, &VBO); // Generate buffer ID
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind buffer to GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); // Apply vertices array to the buffer

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Mesh::~Mesh() {}

void Mesh::draw(Window& window, Shader& shader, glm::vec3 position, glm::vec3 scale, float rotation) {
  // Create the model matrix
  glm::mat4 model(1.f);

  model = glm::translate(model, position);
  model = glm::scale(model, scale);
  model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));

  // Create the orthographic projection matrix
  glm::mat4 projection = glm::ortho(0.f, static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight()), 0.f, -1.f, 1.f);

  // Set the matrices in the shader
  shader.setMat4("model", model);
  shader.setMat4("projection", projection);

  // Draw the mesh
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}

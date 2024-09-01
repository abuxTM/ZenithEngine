#include "ZenithEngine/mesh/mesh.h"
#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/window.h"
#ifndef __APPLE__
#include <GL/gl.h>
#endif
#ifdef __APPLE__
#include <glad/glad.h>
#endif
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

Mesh::Mesh(GLfloat *vertices, GLsizeiptr verticesSize, GLuint *indices,
           GLsizeiptr indicesSize, GLsizei stride) {
  // Generate and bind Vertex Array Object (VAO)
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate and bind Vertex Buffer Object (VBO)
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // Texture coords attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Normal attribute
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // Face ID attribute
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  // Generate and bind Element Buffer Object (EBO) if indices are provided
  if (indices && indicesSize > 0) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    useEBO = true;
  } else {
    useEBO = false;
  }

  // Unbind VAO and VBO (EBO will be unbound automatically)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// It's like Meash.clean but better
Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::draw(Window &window, Shader &shader, glm::vec3 position,
                glm::vec2 scale, float rotation, glm::vec2 texRepeat) {
  // Create the model matrix
  glm::mat4 model(1.f);

  model = glm::translate(model, position);
  model =
      glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale, 1.f));

  shader.setMat4("model", model);
  shader.setMat4("view", glm::mat4(1.f));
  shader.setMat4("projection",
                 glm::ortho(0.f, static_cast<float>(window.getWidth()),
                            static_cast<float>(window.getHeight()), 0.f));
  shader.setVec2("texRepeat", texRepeat);

  // Set color if not textured
  if (!textured)
    shader.setVec3("aColor", color);

  // Draw the mesh
  if (useEBO) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  } else {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }
}

void Mesh::draw(Window &window, Camera &camera, Shader &shader,
                glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
                glm::vec2 texRepeat) {
  // Create the model matrix
  glm::mat4 model(1.f);

  model = glm::translate(model, position);
  model =
      glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);

  shader.setMat4("projection", camera.projection);
  shader.setMat4("view", camera.view);

  shader.setMat4("model", model);
  shader.setVec2("texRepeat", texRepeat);

  // Set color if not textured
  if (!textured)
    shader.setVec3("aColor", color);

  // Draw the mesh
  if (useEBO) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  } else {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }
}

void Mesh::updateMesh(GLfloat *vertices, GLsizeiptr verticesSize,
                      GLuint *indices, GLsizeiptr indicesSize) {
  if (indices == 0 && indicesSize == 0)
    useEBO = false;
  else
    useEBO = true;

  // Bind the VBO (Vertex Buffer Object)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Update the vertex data in the VBO
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

  // Unbind the VBO (Optional: for safety and state management)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Bind the EBO (Element Buffer Object)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // Update the index data in the EBO
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

  // Unbind the EBO (Optional: for safety and state management)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

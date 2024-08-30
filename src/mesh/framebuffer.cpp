#include "ZenithEngine/mesh/framebuffer.h"
#include "ZenithEngine/core/texture.h"
#include "ZenithEngine/core/window.h"
#include <glm/fwd.hpp>
#include <iostream>

static float vertices[] = {
  -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
   1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
};

Framebuffer::Framebuffer(int width, int height) : texture(width, height) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // Framebuffer setup
  glGenFramebuffers(1, &ID);
  glBindFramebuffer(GL_FRAMEBUFFER, ID);

  // Create a renderbuffer object for depth and stencil attachment (optional)
  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

  texture.bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.ID, 0);
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &ID);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteTextures(1, &texture.ID);
}

void Framebuffer::draw(Window& window, Shader& shader) {
  texture.bind();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void Framebuffer::resize(int width, int height) {
  // Bind the framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, ID);
  
  // Resize the texture
  glBindTexture(GL_TEXTURE_2D, texture.ID);  // Bind the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  
  // Resize the renderbuffer if used
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  
  // Reattach the texture to the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.ID, 0);
  
  // Check framebuffer status
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }
  
  // Unbind the framebuffer and texture
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::attachTexture(Texture& texture) {
  glBindFramebuffer(GL_FRAMEBUFFER, ID);  // Bind the framebuffer
  glBindTexture(GL_TEXTURE_2D, texture.ID);  // Ensure the texture is bound
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.ID, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Unbind the framebuffer
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

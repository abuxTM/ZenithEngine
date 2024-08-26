#include "glad/glad.h"
#include "mesh/framebuffer.h"
#include <iostream>

static float vertices[] = {
  -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
   1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
};

Framebuffer::Framebuffer(int width, int height) {
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

  // Check if the framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }
}

void Framebuffer::draw() {
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

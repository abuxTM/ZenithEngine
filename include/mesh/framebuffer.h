#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad/glad.h"

class Framebuffer {
public:
  Framebuffer(int width=800, int height=800);

  void draw();

  void bind();
  void unbind();

  GLuint ID;

private:
  GLuint VAO, VBO, RBO;
};

#endif

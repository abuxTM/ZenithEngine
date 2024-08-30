#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/texture.h"
#include "ZenithEngine/core/window.h"
#include "ZenithEngine/libs/glad/glad.h"

class Framebuffer {
public:
  Framebuffer(int width=800, int height=800);
  ~Framebuffer();

  void draw(Window& window, Shader& shader);
  void attachTexture(Texture& texture);
  void resize(int newWidth, int newHeight);

  void bind();
  void unbind();

  GLuint ID;

private:
  GLuint VAO, VBO, RBO;
  Texture texture;
};

#endif

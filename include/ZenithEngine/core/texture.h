#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef __APPLE__
#include <GL/gl.h>
#endif
#ifdef __APPLE__
#include <glad/glad.h>
#endif

#include <string>

class Texture {
public:
  Texture(int width, int height);
  Texture(const std::string path);
  ~Texture();

  void bind();

  GLuint ID;
  int width, height;
};

#endif

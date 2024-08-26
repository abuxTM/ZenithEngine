#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
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

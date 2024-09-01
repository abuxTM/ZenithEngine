#ifndef SKYBOX_H
#define SKYBOX_H

#include "ZenithEngine/core/camera.h"
#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/window.h"
#ifndef __APPLE__
#include <GL/gl.h>
#endif
#ifdef __APPLE__
#include <glad/glad.h>
#endif
#include <string>

class Skybox {
public:
  Skybox(std::string faces[6]);
  ~Skybox();

  void draw(Window &window, Camera &camera, Shader &shader);

private:
  GLuint VAO, VBO, EBO;
  GLuint texture;
};

#endif

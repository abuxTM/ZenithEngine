#ifndef SKYBOX_H
#define SKYBOX_H

#include "ZenithEngine/core/camera.h"
#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/window.h"
#include <GL/gl.h>
#include <string>

class Skybox {
public:
  Skybox(std::string faces[6]);
  ~Skybox();

  void draw(Window& window, Camera& camera, Shader& shader);

private:
	GLuint VAO, VBO, EBO;
  GLuint texture;
};

#endif

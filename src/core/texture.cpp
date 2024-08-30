#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/texture.h"
#include "ZenithEngine/libs/stb/stb_image.h"
#include <iostream>

Texture::Texture(int width, int height) {
  // Create a texture
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string path) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  // Set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate the texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (data) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    else {
      std::cout << "Unsupported texture format for texture: " << path << std::endl;
      stbi_image_free(data);
      return;
    }

    // Specify the texture with the correct format
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture: " << path << std::endl;
  }

  // Free image data after creating OpenGL texture
  stbi_image_free(data);
}


Texture::~Texture() {}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, ID); }

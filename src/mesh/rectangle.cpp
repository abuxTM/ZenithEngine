#include "ZenithEngine/mesh/rectangle.h"
#include "ZenithEngine/mesh/mesh.h"

static float vertices[] = {
  // First triangle
  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // Bottom-left corner
   0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right corner
   0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right corner

  // Second triangle
   0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right corner
  -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // Top-left corner
  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f   // Bottom-left corner
};

Rectangle::Rectangle() : Mesh(vertices, sizeof(vertices), 0, 0, 5) {}

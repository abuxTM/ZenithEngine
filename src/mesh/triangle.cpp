#include "ZenithEngine/mesh/triangle.h"
#include "ZenithEngine/mesh/mesh.h"

static float vertices[] = {
  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // Bottom-left corner of the texture
   0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right corner of the texture
   0.0f,  0.5f, 0.0f,  0.5f, 1.0f   // Top-center of the texture
};

Triangle::Triangle() : Mesh(vertices, sizeof(vertices), 0, 0, 5) {}

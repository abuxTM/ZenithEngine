#include "ZenithEngine/mesh/cube.h"
#include "ZenithEngine/mesh/mesh.h"

static GLfloat vertices[] = {
 // Positions           // Texture        // Normals             // ID
  // Front face
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,       0.0f,  1.0f,  1.0f,    0.f,  // Bottom-left
   0.5f, -0.5f,  0.5f,   1.0f, 0.0f,       0.0f,  1.0f,  1.0f,    0.f,  // Bottom-right
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f,       0.0f,  1.0f,  1.0f,    0.f,  // Top-right
  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,       0.0f,  1.0f,  1.0f,    0.f,  // Top-left

  // Back face
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       0.0f,  0.0f, -1.0f,    1.f,  // Bottom-left
  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,       0.0f,  0.0f, -1.0f,    1.f,  // Top-left
   0.5f,  0.5f, -0.5f,   1.0f, 1.0f,       0.0f,  0.0f, -1.0f,    1.f,  // Top-right
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f,       0.0f,  0.0f, -1.0f,    1.f,  // Bottom-right

  // Bottom face
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       0.0f, -1.0f,  0.0f,    2.f,  // Bottom-left
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f,       0.0f, -1.0f,  0.0f,    2.f,  // Bottom-right
   0.5f, -0.5f,  0.5f,   1.0f, 1.0f,       0.0f, -1.0f,  0.0f,    2.f,  // Top-right
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,       0.0f, -1.0f,  0.0f,    2.f,  // Top-left

  // Top face
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,       0.0f,  1.0f,  0.0f,    3.f,  // Bottom-left
  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,       0.0f,  1.0f,  0.0f,    3.f,  // Top-left
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f,       0.0f,  1.0f,  0.0f,    3.f,  // Top-right
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f,       0.0f,  1.0f,  0.0f,    3.f,  // Bottom-right

  // Left face
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,      -1.0f,  0.0f,  0.0f,    4.f,  // Bottom-left
  -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,      -1.0f,  0.0f,  0.0f,    4.f,  // Bottom-right
  -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,      -1.0f,  0.0f,  0.0f,    4.f,  // Top-right
  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,    4.f,  // Top-left

  // Right face
   0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       1.0f,  0.0f,  0.0f,    5.f,  // Bottom-left
   0.5f,  0.5f, -0.5f,   0.0f, 1.0f,       1.0f,  0.0f,  0.0f,    5.f,  // Top-left
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f,       1.0f,  0.0f,  0.0f,    5.f,  // Top-right
   0.5f, -0.5f,  0.5f,   1.0f, 0.0f,       1.0f,  0.0f,  0.0f,    5.f,   // Bottom-right
};

static GLuint indices[] = {
  // Front face
  0, 2, 1, 2, 0, 3,
  // Back face
  4, 6, 5, 6, 4, 7,
  // Bottom face
  8, 10, 9, 10, 8, 11,
  // Top face
  12, 14, 13, 14, 12, 15,
  // Left face
  16, 18, 17, 18, 16, 19,
  // Right face
  20, 22, 21, 22, 20, 23
};

Cube::Cube() : Mesh(vertices, sizeof(vertices), indices, sizeof(indices), 9) {}

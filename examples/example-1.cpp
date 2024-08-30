#include "core/window.h"
#include "core/texture.h"
#include "imgui.h"
#include "mesh/rectangle.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

int main (int argc, char *argv[]) {
  // Window Creation
  Window window("Hengine", 1280, 720);

  // Shaders
  Shader shader("shaders/vert.glsl", "shaders/frag.glsl");

  // Shader Config
  shader.use();
  shader.setBool("textured", true);

  // Textures
  Texture tileTex("assets/floor.png");
  Texture rifleTex("assets/rifle.png");

  // Objects
  Rectangle mesh;
  Rectangle rifleMesh;

  // Object variable's
  glm::vec3 position(1280.f/2.f, 720.f/2.f, 0.f);
  float rotation = 0.f;
  glm::vec3 scale(100.f);
  glm::vec2 texRepeat(1.f);

  // Main game loop
  while (!window.shouldClose()) {
    // Self explanatory...
    window.setBackgroundColor(glm::vec3(20.f)/255.f);
    window.clear();

    // Add texture to the object
    rifleTex.bind();
    rifleMesh.draw(window, shader, position, glm::vec2(300.f/2.f, 100.f/2.f), rotation, texRepeat);
    // Same here
    tileTex.bind();
    mesh.draw(window, shader, position, scale, rotation, texRepeat);

    // UI using ImGui if you couldn't tell...
    window.uiClear();
    ImGui::DragFloat2("Position", glm::value_ptr(position));
    ImGui::DragFloat("Rotation", &rotation);
    ImGui::DragFloat2("Scale", glm::value_ptr(scale));
    ImGui::DragFloat2("Texture Repeat", glm::value_ptr(texRepeat));
    window.uiPresent();

    window.present();
  }
  
  return 0;
}

#include "mesh/framebuffer.h"
#include "core/shader.h"
#include "core/texture.h"
#include "core/window.h"
#include "imgui.h"
#include "libs/stb/stb_image.h"
#include "mesh/triangle.h"
#include <GL/gl.h>
#include <cmath>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <imgui.h>

int main (int argc, char *argv[]) {
  // Window
  // #################################################################################
  Window window("Maskot-Lite", 800, 800);
 
  // Shaders
  // #################################################################################
  Shader shader("shaders/vert.glsl", "shaders/frag.glsl");
  Shader screenShader("shaders/screen_vert.glsl", "shaders/screen_frag.glsl");

  // Textures
  // #################################################################################
  Texture tileTexture("assets/tile.png");
  Texture grassTexture("assets/grass.png");

  // Meshes
  // #################################################################################
  Triangle triangle;

  // Framebuffer
  // #################################################################################
  Framebuffer framebuffer;
  Texture texture(800, 800);
  framebuffer.unbind();

  // Useless stuff
  // #################################################################################
  bool linefy = false;
  GLuint nTexture = 0;
  glm::vec2 position(800/2, 800/2);
  glm::vec2 scale(400.f);
  float rotation = 180.f;

  GLfloat colorData[3];
  glGetUniformfv(shader.ID, glGetUniformLocation(shader.ID, "aColor"), colorData);
  glm::vec3 color(colorData[0], colorData[1], colorData[2]);

  glm::vec3 background(20.f/255.f);

  // Main engine loop
  // #################################################################################
  while (!window.shouldClose()) {
    framebuffer.bind(); // Optional
    glEnable(GL_DEPTH_TEST); // Optional
    glEnable(GL_CULL_FACE); // Optional
    window.setBackgroundColor(background); // Optional
    window.clear();

    // Inputs
    // #################################################################################
    if (window.isKeyPressed(GLFW_KEY_C)) break;
    if (window.isKeyPressed(GLFW_KEY_F)) linefy = !linefy;
    linefy ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader.use();

    // Drawing mesh
    // #################################################################################
    glBindTexture(GL_TEXTURE_2D, nTexture);
    triangle.draw(window, shader, glm::vec3(position, 0.f), glm::vec3(scale, 1.f), rotation);

    // UI using IMGui
    // #################################################################################
    window.uiClear();
    ImGui::DragFloat2("Position", glm::value_ptr(position), 0.1f);
    ImGui::DragFloat2("Scale", glm::value_ptr(scale));
    ImGui::DragFloat("Rotation", &rotation);
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) { shader.setVec3("aColor", color); }
    ImGui::ColorEdit3("Background", glm::value_ptr(background));
    if (ImGui::ImageButton("a", (ImTextureID)tileTexture.ID, ImVec2(32.f, 32.f), ImVec2(0, 1), ImVec2(1, 0))) {
      shader.setBool("textured", true);
      nTexture = tileTexture.ID;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton("b", (ImTextureID)grassTexture.ID, ImVec2(32.f, 32.f), ImVec2(0, 1), ImVec2(1, 0))) {
      shader.setBool("textured", true);
      nTexture = grassTexture.ID;
    }

    if (ImGui::Button("Textured")) {
      shader.setBool("textured", !shader.getBool("textured"));
    }
    window.uiPresent();

    // Framebuffer drawing
    // #################################################################################
    framebuffer.unbind();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    window.setBackgroundColor(20, 20, 20);
    window.clear();

    screenShader.use();
    texture.bind();
    framebuffer.draw();

    window.checkErrors();
    window.present();
  }
  
  return 0;
}

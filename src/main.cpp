#include "core/shader.h"
#include "core/window.h"
#include "mesh/triangle.h"
#include <glm/fwd.hpp>

int main (int argc, char *argv[]) {
  Window window;

  Shader shader("shaders/vert.glsl", "shaders/frag.glsl");
  Triangle mesh;

  while (!window.shouldClose()) {
    window.setBackgroundColor(20, 20, 20);
    window.clear();

    if (window.isKeyPressed(GLFW_KEY_C)) break;

    shader.use();
    mesh.draw(window, shader, glm::vec3(window.getWidth()/2.f, window.getHeight()/2.f, 0.f), glm::vec3(250, 250, 0), 180);

    window.present();
  }
  
  return 0;
}

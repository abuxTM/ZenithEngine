#ifndef WINDOW_H
#define WINDOW_H

#include <libs/glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window {
public:
  Window(const char* title="ZenithEngine", int width=1280, int height=720, bool dynamic=false);
  ~Window();

  void clear();
  void present();
  void uiClear();
  void uiPresent();
  void setBackgroundColor(glm::vec3 value) { glClearColor(value.x, value.y, value.y, 1.f); }
  void setBackgroundColor(int r, int g, int b) { glClearColor((float)r/255.f, (float)g/255.f, (float)b/255.f, 1.f); }
  bool shouldClose() { return glfwWindowShouldClose(window); }

  [[nodiscard]] inline int32_t getWidth() const { return m_width; }
  void setWidth(int32_t width) { m_width = width; }
  [[nodiscard]] inline int32_t getHeight() const { return m_height; }
  void setHeight(int32_t height) { m_height = height; }
  int32_t checkErrors(bool log=true);

  bool isKeyDown(int key);
  bool isKeyPressed(int key);
  void getCursorPos(double* x, double* y) { glfwGetCursorPos(window, x, y); }
  void setCursorVis(bool value=true) { glfwSetInputMode(window, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }

  bool camera = false;

  float deltaTime;

private:
  GLFWwindow* window;
  int m_width;
  int m_height;
  double m_lastTime = glfwGetTime();

  static void onResized(GLFWwindow *, int32_t width, int32_t height);
};

#endif

#include "core/window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, bool> keyStates;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    keyStates[key] = true;
  } else if (action == GLFW_RELEASE) {
    keyStates[key] = false;
  }
}

Window::Window(const char* title, int width, int height, bool dynamic)
  : window(nullptr), m_width(width), m_height(height) {
  // Initialize GLFW Library
  glfwInit();

  // Add hints to the GLFW
  glfwWindowHint(GLFW_RESIZABLE, dynamic);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);

  // Create new window using GLFW Library
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create window!" << std::endl;
    glfwTerminate();
    return;
  }

  // Create context out of the window
  glfwMakeContextCurrent(window);

  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, onResized);
  glfwSetKeyCallback(window, keyCallback);

  // Load OpenGL using Glad
  gladLoadGL();
  glViewport(0, 0, width, height);

  while (glGetError() != GL_NO_ERROR) { }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT); // Change this to GL_BACK if needed
  glFrontFace(GL_CCW);
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  /*io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows*/

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init("#version 330");
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::clear() {
  double currentTime = glfwGetTime();
  deltaTime = currentTime - m_lastTime;
  m_lastTime = currentTime;

  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::present() { glfwSwapBuffers(window); }

void Window::uiClear() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, NULL, ImGuiDockNodeFlags_PassthruCentralNode);
}

void Window::uiPresent() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::UpdatePlatformWindows();
  ImGui::RenderPlatformWindowsDefault();
}

bool Window::isKeyDown(int key) { return (glfwGetKey(window, key) == GLFW_PRESS); }

bool Window::isKeyPressed(int key) {
  if (keyStates[key]) {
    keyStates[key] = false;
    return true;
  }
  return false;
}

int32_t Window::checkErrors(bool log) {
  int32_t error = glGetError();
  if (error && log) std::cout << "OpenGL::ERROR: " << error << std::endl;
  return error;
}

void Window::onResized(GLFWwindow* glfwWindow, int32_t width, int32_t height) {
  Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    
  if (window) {
    window->setHeight(height);
    window->setWidth(width);
    glViewport(0, 0, window->getWidth(), window->getHeight());
  }
}

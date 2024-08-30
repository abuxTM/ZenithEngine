#include "ZenithEngine/core/camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
  : position(position), worldUp(up), yaw(yaw), pitch(pitch), view(glm::mat4(1.f)), FOV(120.f), speed(3.f), sensitivity(0.1f), tiltAmount(4.f), tiltEnabled(true), tiltInvert(false), isLocked(true), friction(0.9f), velocity(glm::vec3(0.0f)) {
  updateCameraVectors();
}

Camera::~Camera() {}

void Camera::update(Window& window) {
  processKeyboard(window);
  if (isLocked) processMouseMovement(window);

  if (window.camera) {
    projection = glm::perspective(glm::radians(FOV), static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()), 0.1f, 2500.0f);
  } else {
    projection = glm::ortho(0.f, static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight()), 0.f, -1.f, 1.f);
  }

  position += velocity * window.deltaTime;
  velocity *= friction; // Apply friction to smooth movement

  // Standard view matrix calculation
  view = glm::lookAt(position, position + direction, up);

  // Apply roll around the fixed world axis (e.g., z-axis)
  glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));

  // Combine the roll matrix with the view matrix
  view = rollMatrix * view;
}

void Camera::processKeyboard(Window& window) {
  glm::vec3 front = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
  if (isLocked) {
    window.setCursorVis();

    if (window.isKeyDown(GLFW_KEY_W)) velocity += speed * front;
    if (window.isKeyDown(GLFW_KEY_S)) velocity -= speed * front;
    if (window.isKeyDown(GLFW_KEY_A)) {
      velocity -= right * speed;
      if (tiltEnabled) roll = glm::mix(roll, tiltInvert ? tiltAmount : -tiltAmount, 0.1f);
    }
    if (window.isKeyDown(GLFW_KEY_D)) {
      velocity += right * speed;
      if (tiltEnabled) roll = glm::mix(roll, tiltInvert ? -tiltAmount : tiltAmount, 0.1f); 
    }
    if (window.isKeyDown(GLFW_KEY_E) || window.isKeyDown(GLFW_KEY_SPACE)) velocity += worldUp * speed;
    if (window.isKeyDown(GLFW_KEY_Q) || window.isKeyDown(GLFW_KEY_LEFT_SHIFT)) velocity -= worldUp * speed;

    if (!window.isKeyDown(GLFW_KEY_A) && !window.isKeyDown(GLFW_KEY_D)) roll = glm::mix(roll, 0.0f, 0.1f);
  } else window.setCursorVis(false);
  
  if (window.isKeyPressed(GLFW_KEY_LEFT_ALT)) isLocked = !isLocked;
}

void Camera::processMouseMovement(Window& window) {
  static bool firstMouse = true;
  static double lastX, lastY;

  double xpos, ypos;
  window.getCursorPos(&xpos, &ypos);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Clamp pitch to avoid screen flipping
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction = glm::normalize(front);

  right = glm::normalize(glm::cross(direction, worldUp));  
  up = glm::normalize(glm::cross(right, direction));
}

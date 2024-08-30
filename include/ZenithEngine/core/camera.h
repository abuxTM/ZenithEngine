#ifndef CAMERA_H
#define CAMERA_H

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "ZenithEngine/core/window.h"
#include "glm/mat4x2.hpp"

class Camera {
public:
  Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.f);
  ~Camera();

  void update(Window& window);

  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  glm::mat4 view;
  glm::mat4 projection;

  float yaw;
  float pitch;
  float roll;
  float tiltAmount;
  bool tiltEnabled;
  bool tiltInvert;
  bool isLocked;

  float FOV;
  float speed;
  float sensitivity;
  float friction;

  glm::vec3 velocity;

private:
  void processKeyboard(Window& window);
  void processMouseMovement(Window& window);
  void updateCameraVectors();
};

#endif

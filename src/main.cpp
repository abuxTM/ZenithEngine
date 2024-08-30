#include "ZenithEngine/core/camera.h"
#include "ZenithEngine/core/shader.h"
#include "ZenithEngine/core/texture.h"
#include "ZenithEngine/core/window.h"
#include "imgui.h"
#include "ZenithEngine/math/raycast.h"
#include "ZenithEngine/mesh/cube.h"
#include "ZenithEngine/mesh/framebuffer.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "ZenithEngine/libs/stb/stb_image.h"
#include "ZenithEngine/mesh/skybox.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Settings {
  struct UI {
    ImVec4 highlight = ImVec4(254.f/255.f, 128.f/255.f, 25.f/255.f, 1.f);
  } ui;
  bool skyboxEnabled = true;
  glm::vec3 background = glm::vec3(0.f);
  glm::vec3 ambientColor = glm::vec3(0.9f);
  float ambientIntensity = 0.4f;
  bool showColliders = false;
};

int main (int argc, char *argv[]) {
  Window window("Masok", 1280, 720, true);
  window.camera = true;

  // Shaders
  Shader shader("shaders/vert.glsl", "shaders/frag.glsl");
  Shader skyboxShader("shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl");
  Shader framebufferShader("shaders/screen_vert.glsl", "shaders/screen_frag.glsl");

  // Shaders config
  shader.use();
  shader.setInt("aTexture", 0);
  shader.setBool("textured", false);
  shader.setBool("lighting", true);

  // Textures
  std::vector<Texture> textures;
  textures.push_back(Texture("assets/tile.png"));
  textures.push_back(Texture("assets/floor.png"));
  textures.push_back(Texture("assets/grass.png"));
  textures.push_back(Texture("assets/dirt.png"));
  textures.push_back(Texture("assets/textures/stone.png"));

  std::string facesCubemap[6] = {
    "assets/skybox/right.jpg",
    "assets/skybox/left.jpg",
    "assets/skybox/top.jpg",
    "assets/skybox/bottom.jpg",
    "assets/skybox/front.jpg",
    "assets/skybox/back.jpg",
  };

  // Objects
  Framebuffer framebuffer(1280, 720);
  Skybox skybox(facesCubemap);
  Camera camera(glm::vec3(1.f));
  Cube mesh;

  Settings settings;
  std::vector<Object> objects;

  unsigned int selectedObject = -1;

  while (!window.shouldClose()) {
    framebuffer.bind();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    window.setBackgroundColor(settings.background);
    window.clear();

    // Inputs
    // ########################################################################################################
    if (window.camera) camera.update(window);
    if (window.isKeyPressed(GLFW_KEY_1)) settings.showColliders = !settings.showColliders;
    if (window.isKeyPressed(GLFW_KEY_F)) {
      RayCast ray(camera.position, glm::normalize(camera.direction), 100.f, objects);

      if (ray.hasHit()) {
        HitTarget target = ray.getHitTarget();
        std::cout << "Hit position: " << glm::to_string(target.position) << std::endl;
        std::cout << "Surface normal: " << glm::to_string(target.normal) << std::endl;
        objects.push_back(Object {
          std::string("Ray"),               // Name
          target.position,                  // Position
          glm::vec3(0.),                    // Rotation
          glm::vec3(1.5f),                  // Scale
          glm::vec3(0.2f, 0.1f, 0.7f),      // Color
          glm::vec2(1.f),                   // Texture Repeat
          textures[0].ID,                   // Texture
        });
      }
    }

    // Rendering
    // ########################################################################################################
    // Render objects
    shader.use();
    for (auto& object : objects) {
      // Draw the base mesh
      shader.setBool("textured", object.textured);
      glBindTexture(GL_TEXTURE_2D, object.texture);
      mesh.color = object.color;
      mesh.draw(window, camera, shader, object.position, object.scale, object.rotation, object.texRepeat);

      // Draw the collider
      if (settings.showColliders) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader.setBool("textured", false);
        mesh.draw(window, camera, shader, object.position, object.scale + 0.3f, object.rotation, object.texRepeat);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }

    // Render skybox
    if (settings.skyboxEnabled) skybox.draw(window, camera, skyboxShader);

    // Render postprocessing
    // ########################################################################################################
    framebuffer.unbind();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    window.setBackgroundColor(20, 20, 20);
    window.clear();

    framebufferShader.use();
    framebuffer.resize(window.getWidth(), window.getHeight());
    framebuffer.draw(window, framebufferShader);

    // UI
    // ########################################################################################################
    if (!camera.isLocked) {
      window.uiClear();
      ImGui::Begin("Objects");
      // ########################################################################################################
      if (ImGui::Button("Create Object"))
        objects.push_back(Object {
          std::string(std::to_string(objects.size())),               // Name
          glm::vec3(0.f, 0.f, 0.f),         // Position
          glm::vec3(0.),                    // Rotation
          glm::vec3(4.f),                   // Scale
          glm::vec3(0.2f, 0.1f, 0.7f),      // Color
          glm::vec2(1.f),                   // Texture Repeat
          textures[0].ID,                   // Texture
        });

      for (unsigned int i = 0; i < objects.size(); i++) {
        char label[32];
        if (objects[i].name.empty()) std::sprintf(label, "ID) #%u", i);
        else std::sprintf(label, "%s", objects[i].name.c_str());
        if (ImGui::Button(label)) {
          selectedObject = i;
        }
      }
      ImGui::End();

      ImGui::Begin("IO");
      // ########################################################################################################
      static std::string buffer = "world.zen";
      char pathBuffer[256];
      std::strncpy(pathBuffer, buffer.c_str(), sizeof(pathBuffer));

      if (ImGui::Button("Save")) {
        // Create or open the file for writing
        std::ofstream f(pathBuffer);

        // Check if the file is open
        if (!f.is_open()) {
            // Handle file opening error
            std::cerr << "Error opening file for writing." << std::endl;
        }

        // Save all objects to the file
        for (const auto& object : objects) {
          f << object.name << "," 
            << object.position.x << "," << object.position.y << "," << object.position.z << ","
            << object.rotation.x << "," << object.rotation.y << "," << object.rotation.z << "," 
            << object.scale.x << "," << object.scale.y << "," << object.scale.z << ","
            << object.color.x << "," << object.color.y << "," << object.color.z << ","
            << object.texRepeat.x << "," << object.texRepeat.y << ","
            << object.texture << ","
            << object.textured << "\n";
        }

        // Close the file
        f.close();
      }
      ImGui::SameLine();
      if (ImGui::Button("Load")) {
        // Will be used to store things
        std::string buffer;
        objects.clear();

        // Read file
        std::ifstream f(pathBuffer);

        // Read file line by line
        while (getline(f, buffer)) {
          std::stringstream ss(buffer);
          Object object;

          // Read name
          getline(ss, object.name, ',');

          // Read position
          ss >> object.position.x; ss.ignore(1, ',');
          ss >> object.position.y; ss.ignore(1, ',');
          ss >> object.position.z; ss.ignore(1, ',');

          // Read rotation
          ss >> object.rotation.x; ss.ignore(1, ',');
          ss >> object.rotation.y; ss.ignore(1, ',');
          ss >> object.rotation.z; ss.ignore(1, ',');

          // Read scale
          ss >> object.scale.x; ss.ignore(1, ',');
          ss >> object.scale.y; ss.ignore(1, ',');
          ss >> object.scale.z; ss.ignore(1, ',');

          // Read color
          ss >> object.color.x; ss.ignore(1, ',');
          ss >> object.color.y; ss.ignore(1, ',');
          ss >> object.color.z; ss.ignore(1, ',');

          // Read texRepeat
          ss >> object.texRepeat.x; ss.ignore(1, ',');
          ss >> object.texRepeat.y; ss.ignore(1, ',');

          // Read texture
          ss >> object.texture; ss.ignore(1, ',');

          // Read textured
          ss >> object.textured;

          // Create a new object with the parsed values
          objects.push_back(Object {
            object.name,
            object.position,
            object.rotation,
            object.scale,
            object.color,
            object.texRepeat,
            object.texture,
            object.textured,
          });
        }

        // Close the file
        f.close();
      }
      if (ImGui::InputText("Path", pathBuffer, sizeof(pathBuffer))) buffer = pathBuffer;
      ImGui::End();

      ImGui::Begin("Inspector");
      // ########################################################################################################
      if (selectedObject < -1) {
        char buffer[256];
        std::strncpy(buffer, objects[selectedObject].name.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) objects[selectedObject].name = std::string(buffer);
        ImGui::TextColored(settings.ui.highlight, "Transform");
        ImGui::DragFloat3("Position", glm::value_ptr(objects[selectedObject].position));
        ImGui::DragFloat3("Rotation", glm::value_ptr(objects[selectedObject].rotation));
        ImGui::DragFloat3("Scale", glm::value_ptr(objects[selectedObject].scale));
        if (objects[selectedObject].textured) {
          ImGui::TextColored(settings.ui.highlight, "Textures");
          for (unsigned int i = 0; i < textures.size(); i++) {
            std::string label = "Texture##" + std::to_string(i);
            if (ImGui::ImageButton(label.c_str(), (ImTextureID)(uintptr_t)textures[i].ID, ImVec2(32.f, 32.f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f))) {
              objects[selectedObject].texture = textures[i].ID;
            }
            if (((i + 1) % 3 != 0 && i != textures.size() - 1)) ImGui::SameLine();
          }
          ImGui::DragFloat2("Texture Repeat", glm::value_ptr(objects[selectedObject].texRepeat));
        } else {
          ImGui::ColorEdit3("Color", glm::value_ptr(objects[selectedObject].color));
        }
        ImGui::Checkbox("Textured", &objects[selectedObject].textured);
        if (ImGui::Button("Delete")) {
          if (selectedObject < objects.size()) {
            objects.erase(objects.begin() + selectedObject);
            if (selectedObject >= objects.size()) selectedObject = objects.size() - 1;
          }
        }
      }
      ImGui::End();

      ImGui::Begin("Config");
      // ########################################################################################################
      ImGui::TextColored(settings.ui.highlight, "Global Config");
      ImGui::Checkbox("Enable 3D Projection", &window.camera);
      ImGui::Checkbox("Enable Skybox", &settings.skyboxEnabled);
      ImGui::ColorEdit3("Background", glm::value_ptr(settings.background));

      ImGui::Spacing();
      ImGui::TextColored(settings.ui.highlight, "Lighting");
      if (ImGui::ColorEdit3("Ambient Color", glm::value_ptr(settings.ambientColor))) {
        shader.use();
        shader.setVec3("lightColor", settings.ambientColor);
      }
      if (ImGui::DragFloat("Ambient Intensity", &settings.ambientIntensity)) {
        shader.use();
        shader.setFloat("ambientIntensity", settings.ambientIntensity);
      }

      ImGui::Spacing();
      ImGui::TextColored(settings.ui.highlight, "Camera Config");
      ImGui::DragFloat("FOV", &camera.FOV);
      ImGui::DragFloat("Speed", &camera.speed);
      ImGui::DragFloat("Tilt Amount", &camera.tiltAmount);
      ImGui::Checkbox("Tilt Enabled", &camera.tiltEnabled);
      ImGui::Checkbox("Tilt Invert", &camera.tiltInvert);

      ImGui::Spacing();
      ImGui::TextColored(settings.ui.highlight, "Postprocessing Config");
      static float vignetteIntensity = framebufferShader.getFloat("vignetteIntensity");
      if (ImGui::DragFloat("Vignette Intensity", &vignetteIntensity)) framebufferShader.setFloat("vignetteIntensity", vignetteIntensity);
      static float vignetteInner = framebufferShader.getFloat("vignetteInner");
      if (ImGui::DragFloat("Vignette Inner", &vignetteInner)) framebufferShader.setFloat("vignetteInner", vignetteInner);
      ImGui::End();
      window.uiPresent();
    }

    window.checkErrors();
    window.present();
  }
  
  return 0;
}

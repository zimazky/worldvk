#include "app.hpp"

#include "camera.hpp"
#include "kbcontroller.hpp"
#include "simplerendersystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>

#define MAX_FRAME_TIME 0.5f

namespace world {

  App::App() { loadGameObjects(); }

  App::~App() = default;

  void App::run() {
    SimpleRenderSystem simpleRenderSystem {device, renderer.getSwapChainRenderPass()};
    Camera camera {};

    auto viewerObject = GameObject::createGameObject();
    KeyboardMovementController cameraController {};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose()) {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
      currentTime = newTime;
      frameTime = glm::min(frameTime, MAX_FRAME_TIME);

      cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
      camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

      float aspect = renderer.getAspectRatio();
      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

      if (auto commandBuffer = renderer.beginFrame()) {
        // begin offscreen shadow pass
        // render shadow casting objects
        // end offscreen shadow pass

        renderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        renderer.endSwapChainRenderPass(commandBuffer);
        renderer.endFrame();
      }
    }
    vkDeviceWaitIdle(device.device());
  }

  void App::loadGameObjects() {
    std::shared_ptr<Model> model = Model::createModelFromFile(device, "assets/models/flat_vase.obj");
    auto flatVase = GameObject::createGameObject();
    flatVase.model = model;
    flatVase.transform.translation = {-0.9f, 0.5f, 2.5f};
    flatVase.transform.scale = glm::vec3 {6.f, 3.f, 6.f};
    gameObjects.push_back(std::move(flatVase));

    model = Model::createModelFromFile(device, "assets/models/smooth_vase.obj");
    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transform.translation = {0.9f, 0.5f, 2.5f};
    smoothVase.transform.scale = glm::vec3 {6.f, 3.f, 6.f};
    gameObjects.push_back(std::move(smoothVase));
  }

} // namespace world
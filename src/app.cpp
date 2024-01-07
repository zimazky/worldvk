#include "app.hpp"

#include "simplerendersystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace world {

  App::App() {
    loadGameObjects();
  }

  App::~App() {}

  void App::run() {
    SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
    while (!window.shouldClose()) {
      glfwPollEvents();

      if(auto commandBuffer = renderer.beginFrame()) {

        // begin offscreen shadow pass
        // render shadow casting objects
        // end offscreen shadow pass

        renderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
        renderer.endSwapChainRenderPass(commandBuffer);
        renderer.endFrame();
      }
    }
    vkDeviceWaitIdle(device.device());
  }

  void App::loadGameObjects() {
    std::vector<Model::Vertex> vertices {
      {{0.f, -0.5f}, {1.f, 0.f, 0.f}},
      {{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
      {{-0.5f, 0.5f}, {0.f, 0.f, 1.f}}
    };
    auto model = std::make_shared<Model>(device, vertices);
    auto triangle = GameObject::createGameObject();
    triangle.model = model;
    triangle.color = {0.1f, 0.8f, 0.1f};
    triangle.transform2d.translation.x = 0.2f;
    triangle.transform2d.scale = {2.f, 0.5f};
    triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
  }

}
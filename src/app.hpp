#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "model.hpp"

#include <memory>
#include <vector>

namespace world {
  class App {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      App();
      ~App();

      App(const App &) = delete;
      App &operator=(const App &) = delete;

      void run();

    private:
      void loadModels();
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void drawFrame();

      Window window {WIDTH, HEIGHT, "Hello Vulkan!"};
      Device device {window};
      SwapChain swapChain{device, window.getExtent()};
      std::unique_ptr<Pipeline> pipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::unique_ptr<Model> model;
  };
}
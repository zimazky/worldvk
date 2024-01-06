#include "app.hpp"

#include <stdexcept>
#include <array>

namespace world {

  App::App() {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  App::~App() {
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
  }

  void App::run() {
    while (!window.shouldClose()) {
      glfwPollEvents();
      drawFrame();
    }
    vkDeviceWaitIdle(device.device());
  }

  void Serpinskiy(const std::array<glm::vec2, 3> &triangle, int depth, std::vector<Model::Vertex> &vertices) {
    if(depth == 0) {
      vertices.push_back(Model::Vertex{triangle[0]});
      vertices.push_back(Model::Vertex{triangle[1]});
      vertices.push_back(Model::Vertex{triangle[2]});
    }
    else {
      const std::array<glm::vec2, 3> triangle1 {
        triangle[0],
        0.5f*(triangle[0]+triangle[1]),
        0.5f*(triangle[0]+triangle[2])
      };
      const std::array<glm::vec2, 3> triangle2 {
        0.5f*(triangle[0]+triangle[1]),
        triangle[1],
        0.5f*(triangle[1]+triangle[2]),
      };
      const std::array<glm::vec2, 3> triangle3 {
        0.5f*(triangle[0]+triangle[2]),
        0.5f*(triangle[1]+triangle[2]),
        triangle[2]
      };
      depth--;
      Serpinskiy(triangle1, depth, vertices);
      Serpinskiy(triangle2, depth, vertices);
      Serpinskiy(triangle3, depth, vertices);
    }
  }

  void App::loadModels() {
    /*
    std::vector<Model::Vertex> vertices {
      {{0.f, -0.5f}},
      {{0.5f, 0.5f}},
      {{-0.5f, 0.5f}}
    };
    */
    const std::array<glm::vec2, 3> triangle {
      glm::vec2{0.f, -0.5f},
      glm::vec2{0.5f, 0.5f},
      glm::vec2{-0.5f, 0.5f}
    };

    std::vector<Model::Vertex> vertices {};
    Serpinskiy(triangle, 5, vertices);

    model = std::make_unique<Model>(device, vertices);
  }

  void App::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline layout");
    }
  }

  void App::createPipeline() {
    auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
    pipelineConfig.renderPass = swapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(
      device,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      pipelineConfig
    );
  }

  void App::createCommandBuffers() {
    commandBuffers.resize(swapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate command buffers");
    }

    for(int i = 0; i < commandBuffers.size(); i++) {
      VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer");
      }
      VkRenderPassBeginInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
      renderPassInfo.renderPass = swapChain.getRenderPass();
      renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);
      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.f};
      clearValues[1].depthStencil = {1.f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      pipeline->bind(commandBuffers[i]);
      model->bind(commandBuffers[i]);
      model->draw(commandBuffers[i]);

      vkCmdEndRenderPass(commandBuffers[i]);
      if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer");
      }
    }
  }

  void App::drawFrame() {
    uint32_t imageIndex;
    auto result = swapChain.acquireNextImage(&imageIndex);

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("Failed to acquire swap chain image");
    }

    result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if(result != VK_SUCCESS) {
      throw std::runtime_error("Failed to present swap chain image");
    }
  }

}
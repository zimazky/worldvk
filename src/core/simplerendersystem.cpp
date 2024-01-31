#include "simplerendersystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>

namespace world {

  struct SimplePushConstantData {
    glm::mat4 transform {1.f};
    glm::mat4 normalMatrix {1.f};

    //    alignas(16) glm::vec3 color;
  };

  SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass) : device {device} {
    createPipelineLayout();
    createPipeline(renderPass);
  }

  SimpleRenderSystem::~SimpleRenderSystem() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

  void SimpleRenderSystem::createPipelineLayout() {
    VkPushConstantRange pushConstantRange {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline layout");
    }
  }

  void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig {};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
  }

  void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera) {
    pipeline->bind(commandBuffer);

    auto projectionView = camera.getProjection() * camera.getView();

    for (auto& obj : gameObjects) {
      SimplePushConstantData push {};
      auto modelMatrix = obj.transform.mat4();
      push.transform = projectionView * modelMatrix;
      push.normalMatrix = obj.transform.normalMatrix();

      vkCmdPushConstants(
        commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push
      );
      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }

} // namespace world
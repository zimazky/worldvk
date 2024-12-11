#pragma once

#include "camera.hpp"
#include "device.hpp"
#include "gameobject.hpp"
#include "pipeline.hpp"

#include <memory>
#include <vector>

namespace world {
  class SimpleRenderSystem : public NonCopyable {
  public:

    SimpleRenderSystem(Device& device, VkRenderPass renderPass);
    ~SimpleRenderSystem() override;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

  private:

    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device& device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout {};
  };
} // namespace world
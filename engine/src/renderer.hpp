#pragma once

#include "device.hpp"
#include "swapchain.hpp"
#include "window.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace world {
  class Renderer : public NonCopyable {
  public:

    Renderer(Window& window, Device& device);
    ~Renderer() override;

    [[nodiscard]] auto getSwapChainRenderPass() const -> VkRenderPass { return swapChain->getRenderPass(); }
    [[nodiscard]] auto getAspectRatio() const -> float { return swapChain->extentAspectRatio(); }
    [[nodiscard]] auto isFrameInProgress() const -> bool { return isFrameStarted; }
    [[nodiscard]] auto getCurrentCommandBuffer() const -> VkCommandBuffer {
      assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      return commandBuffers[currentFrameIndex];
    }

    [[nodiscard]] auto getFrameIndex() const -> int {
      assert(isFrameStarted && "Cannot get frame index when frame not in progress");
      return currentFrameIndex;
    }

    auto beginFrame() -> VkCommandBuffer;
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  private:

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    Window& window;
    Device& device;
    std::unique_ptr<SwapChain> swapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex {};
    int currentFrameIndex {0};
    bool isFrameStarted {false};
  };
} // namespace world
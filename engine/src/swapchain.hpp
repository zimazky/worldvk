#pragma once

#include "device.hpp"

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace world {

  class SwapChain : public NonCopyable {
  public:

    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    SwapChain(Device& deviceRef, VkExtent2D windowExtent);
    SwapChain(Device& deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
    ~SwapChain() override;

    auto getFrameBuffer(int index) -> VkFramebuffer { return swapChainFramebuffers[index]; }
    auto getRenderPass() -> VkRenderPass { return renderPass; }
    auto getImageView(int index) -> VkImageView { return swapChainImageViews[index]; }
    auto imageCount() -> size_t { return swapChainImages.size(); }
    auto getSwapChainImageFormat() -> VkFormat { return swapChainImageFormat; }
    auto getSwapChainExtent() -> VkExtent2D { return swapChainExtent; }
    auto width() -> uint32_t { return swapChainExtent.width; }
    auto height() -> uint32_t { return swapChainExtent.height; }

    auto extentAspectRatio() -> float { return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height); }
    auto findDepthFormat() -> VkFormat;

    auto acquireNextImage(uint32_t* imageIndex) -> VkResult;
    auto submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex) -> VkResult;

    [[nodiscard]] auto compareSwapFormats(const SwapChain& swapChain) const -> bool {
      return swapChain.swapChainDepthFormat == swapChainDepthFormat && swapChain.swapChainImageFormat == swapChainImageFormat;
    }

  private:

    void init();
    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    // Helper functions
    auto chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) -> VkSurfaceFormatKHR;
    auto chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) -> VkPresentModeKHR;
    auto chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) -> VkExtent2D;

    VkFormat swapChainImageFormat {};
    VkFormat swapChainDepthFormat {};
    VkExtent2D swapChainExtent {};

    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass renderPass {};

    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemorys;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    Device& device;
    VkExtent2D windowExtent;

    VkSwapchainKHR swapChain {};
    std::shared_ptr<SwapChain> oldSwapChain;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
  };
} // namespace world
#pragma once

#include "noncopyable.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace world {
  class Window : public NonCopyable {
  public:

    Window(int w, int h, std::string name);
    ~Window() override;

    auto shouldClose() -> bool { return glfwWindowShouldClose(window); }
    auto getExtent() -> VkExtent2D { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    auto wasWindowResized() -> bool { return framebufferResized; }
    void resetWindowResizedFlag() { framebufferResized = false; }
    [[nodiscard]] auto getGLFWwindow() const -> GLFWwindow* { return window; }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  private:

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void initWindow();
    int width;
    int height;
    bool framebufferResized = false;
    std::string windowName;
    GLFWwindow* window {};
  };
} // namespace world
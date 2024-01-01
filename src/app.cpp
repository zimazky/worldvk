#include "app.hpp"

namespace world {
  void App::run() {
    while (!window.shouldClose()) {
      glfwPollEvents();
    }
  }
}
#pragma once

#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace world {

  struct TransformComponent {
    glm::vec3 translation {};
    glm::vec3 scale {1.f, 1.f, 1.f};
    glm::vec3 rotation {};

    // translate * Ry * Rx * Rz * scale
    auto mat4() -> glm::mat4;
    auto normalMatrix() -> glm::mat3;
  };

  class GameObject {
  public:

    using id_t = unsigned int;

    static auto createGameObject() -> GameObject {
      static id_t currentId = 0;
      return GameObject {currentId++};
    }

    GameObject(const GameObject&) = delete;
    auto operator=(const GameObject&) -> GameObject& = delete;
    GameObject(GameObject&&) = default;
    auto operator=(GameObject&&) -> GameObject& = default;

    auto getId() -> id_t { return id; }

    std::shared_ptr<Model> model {};
    glm::vec3 color {};
    TransformComponent transform {};

  private:

    GameObject(id_t objId) : id {objId} {}

    id_t id;
  };

} // namespace world
#pragma once

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace world {
  class Model : public NonCopyable {
  public:

    struct Vertex {
      glm::vec3 position;
      glm::vec3 color;
      glm::vec3 normal {};
      glm::vec2 uv {};
      static auto getBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>;
      static auto getAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>;

      auto operator==(const Vertex& other) const -> bool {
        return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
      }
    };

    struct Builder {
      std::vector<Vertex> vertices {};
      std::vector<uint32_t> indices {};

      void loadModel(const std::string& filepath);
    };

    Model(Device& device, const Model::Builder& builder);
    ~Model() override;

    static auto createModelFromFile(Device& device, const std::string& filepath) -> std::unique_ptr<Model>;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:

    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    Device& device;
    VkBuffer vertexBuffer {};
    VkDeviceMemory vertexBufferMemory {};
    uint32_t vertexCount {};

    bool hasIndexBuffer = false;
    VkBuffer indexBuffer {};
    VkDeviceMemory indexBufferMemory {};
    uint32_t indexCount {};
  };
} // namespace world
#pragma once
#include "BaseMesh.h"
#include "PerspectiveCamera.h"

struct DirectionalLightStruct {
    alignas(16) glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    alignas(16) glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    alignas(16) glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    alignas(16) glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
};

struct PointLightStruct {
    alignas(16) glm::vec3 position = glm::vec3(0.0f);

    alignas(4) float constant = 1.0f;
    alignas(4) float linear = 0.09f;
    alignas(4) float quadratic = 0.032f;

    alignas(16) glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    alignas(16) glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    alignas(16) glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    alignas(4)  int InUseFlag = 0;
};

struct SpotLightStruct {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    alignas(4) float cutOff;
    alignas(4) float outerCutOff;

    alignas(4) float constant;
    alignas(4) float linear;
    alignas(4) float quadratic;

    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

struct LightBufferObject
{
    DirectionalLightStruct dLight;
    PointLightStruct pLight[128];
    SpotLightStruct sLight;
    alignas(16) glm::vec3 viewPos;
};

struct MapBits
{
    alignas(4) int UseDiffuseMapBit = 0;
    alignas(4) int UseSpecularMapBit = 0;
    alignas(4) int UseNormalMapBit = 0;
    alignas(4) int UseDepthMapBit = 1;
    alignas(4) int UseAlphaMapBit = 0;
    alignas(4) int UseEmissionMapBit = 0;
    alignas(4) int UseSkyBoxBit = 0;
};

struct VertexMatrixObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    glm::mat4 BoneTransform[300];
};

struct Empty
{
    alignas(4) float empty;
};

struct CustomBuffer
{
    unsigned int ByteSize;
    VulkanUniformBuffer customBuffer;
};

class Mesh : public BaseMesh
{
private:
    std::string MeshName;
    int MeshID = -1;
    unsigned int NodeId = -1;
    unsigned int BoneCount = 0;
    glm::mat4 TransformMatrix = glm::mat4(1.0f);
    std::shared_ptr<Mesh> ParentMesh;
    std::vector<std::shared_ptr<Mesh>> ChildrenMesh;

    VertexMatrixObject ubo;

protected:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;
    CustomBuffer ExtendedMeshProperitesBuffer;

    void CreateMaterialProperties(MeshTextures textures);

    virtual void CreateUniformBuffers(VulkanEngine& engine);
    virtual void CreateDescriptorPool(VulkanEngine& engine);
    virtual void CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout);
    virtual void UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, void* CustomBufferinfo = nullptr);
    virtual void UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);

public:
    MeshProperties properites;

    Mesh();
    Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, int renderFlags);
    Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, int renderFlags);
    Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer, int renderFlags);
    Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const MeshData& meshData, VkDescriptorSetLayout layout, int renderFlags);
    Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, VkDescriptorSetLayout& layout, int renderFlags);
    Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer, VkDescriptorSetLayout& layout, int renderFlags);
    ~Mesh();

    void SetTransformMatrix(glm::mat4 NewTranformMatrix);

    virtual void Update(VulkanEngine& engine) override;
    virtual void Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, glm::mat4 ModelMatrix = glm::mat4(1.0f), void* CustomBufferinfo = nullptr);
    virtual void Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, const std::vector<std::shared_ptr<Bone>>& BoneList, glm::mat4 ModelMatrix = glm::mat4(1.0f), void* CustomBufferinfo = nullptr);
    virtual void ScreenResizeUpdate(VulkanEngine& engine, VkDescriptorSetLayout& layout);
    virtual void Destory(VulkanEngine& engine) override;

    std::string GetMeshName() { return MeshName; }
    unsigned int GetNodeId() { return NodeId; }
    glm::mat4 GetTransformMatrix() { return TransformMatrix; }
};
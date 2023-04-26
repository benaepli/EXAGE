#pragma once

#include <filesystem>
#include <optional>

#include "exage/Core/Core.h"
#include "exage/Graphics/CommandBuffer.h"
#include "exage/Renderer/Scene/AssetCache.h"
#include "exage/Renderer/Scene/Loader/Errors.h"
#include "exage/Renderer/Scene/Material.h"
#include "exage/Renderer/Scene/Mesh.h"
#include "exage/Renderer/Scene/SceneBuffer.h"
#include "exage/Scene/Scene.h"

namespace exage::Renderer
{
    [[nodiscard]] EXAGE_EXPORT auto loadTexture(const std::filesystem::path& path) noexcept
        -> tl::expected<Texture, AssetLoadError>;

    [[nodiscard]] EXAGE_EXPORT auto loadMaterial(const std::filesystem::path& path) noexcept
        -> tl::expected<Material, AssetLoadError>;

    [[nodiscard]] EXAGE_EXPORT auto loadMesh(const std::filesystem::path& path) noexcept
        -> tl::expected<Mesh, AssetLoadError>;

    struct TextureUploadOptions
    {
        Graphics::Context& context;
        Graphics::CommandBuffer& commandBuffer;
        Graphics::ResourceManager* resourceManager = nullptr;

        Graphics::Texture::Layout layout = Graphics::Texture::Layout::eShaderReadOnly;
        Graphics::Access access = Graphics::AccessFlags::eShaderRead;
        Graphics::PipelineStage pipelineStage = Graphics::PipelineStageFlags::eFragmentShader;
    };

    struct MaterialUploadOptions
    {
        Graphics::Context& context;
        Graphics::CommandBuffer& commandBuffer;
        Graphics::ResourceManager* resourceManager = nullptr;

        Graphics::Access access = Graphics::AccessFlags::eShaderRead;
        Graphics::PipelineStage pipelineStage = Graphics::PipelineStageFlags::eFragmentShader;
    };

    struct MeshUploadOptions
    {
        Graphics::Context& context;
        Graphics::CommandBuffer& commandBuffer;
        SceneBuffer& sceneBuffer;

        Graphics::Access access = Graphics::AccessFlags::eShaderRead;
        Graphics::PipelineStage pipelineStage = Graphics::PipelineStageFlags::eFragmentShader;
    };

    [[nodiscard]] EXAGE_EXPORT auto uploadTexture(const Texture& texture,
                                                  const TextureUploadOptions& options) noexcept
        -> GPUTexture;

    [[nodiscard]] EXAGE_EXPORT auto uploadMaterial(const Material& material,
                                                   const MaterialUploadOptions& options) noexcept

        -> GPUMaterial;

    [[nodiscard]] EXAGE_EXPORT auto uploadMesh(const Mesh& mesh,
                                               const MeshUploadOptions& options) noexcept
        -> GPUMesh;

}  // namespace exage::Renderer

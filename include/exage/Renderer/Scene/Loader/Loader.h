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
    struct TextureLoadOptions
    {
        std::filesystem::path path;

        Graphics::Texture::Layout layout = Graphics::Texture::Layout::eShaderReadOnly;
        Graphics::ResourceManager* resourceManager = nullptr;

        Graphics::Context* context = nullptr;
        Graphics::CommandBuffer* commandBuffer = nullptr;
        Graphics::Access access = Graphics::AccessFlags::eShaderRead;
        Graphics::PipelineStage pipelineStage = Graphics::PipelineStageFlags::eFragmentShader;
    };

    [[nodiscard]] EXAGE_EXPORT auto loadTexture(const TextureLoadOptions& options) noexcept
        -> tl::expected<Texture, AssetLoadError>;

    struct MaterialLoadOptions
    {
        std::filesystem::path path;
        // ALERT: Material loading does not load textures.
    };

    EXAGE_EXPORT auto loadMaterial(const MaterialLoadOptions& options) noexcept
        -> tl::expected<Material, AssetLoadError>;

    struct MeshLoadOptions
    {
        std::filesystem::path path;

        SceneBuffer* sceneBuffer = nullptr;

        Graphics::Context* context = nullptr;
        Graphics::CommandBuffer* commandBuffer = nullptr;
        Graphics::Access access = Graphics::AccessFlags::eShaderRead;
        Graphics::PipelineStage pipelineStage = Graphics::PipelineStageFlags::eFragmentShader;

        // ALERT: Mesh loading does not load materials.
    };

    EXAGE_EXPORT auto loadMesh(const MeshLoadOptions& options) noexcept
        -> tl::expected<Mesh, AssetLoadError>;

}  // namespace exage::Renderer

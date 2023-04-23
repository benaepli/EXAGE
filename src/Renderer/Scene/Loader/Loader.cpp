#include <fstream>

#include "exage/Renderer/Scene/Loader/Loader.h"

#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>

namespace exage::Renderer
{
    EXAGE_EXPORT auto loadTexture(const TextureLoadOptions& options) noexcept
        -> tl::expected<Texture, AssetLoadError>
    {
        if (!std::filesystem::exists(options.path))
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        if (options.path.extension() != TEXTURE_EXTENSION)
        {
            return tl::make_unexpected(FileFormatError {});
        }

        std::ifstream file(options.path, std::ios::binary);

        if (!file.is_open())
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        Texture texture;
        cereal::BinaryInputArchive archive(file);
        archive(texture);

        if (!options.context)
        {
            return texture;
        }

        debugAssume(options.commandBuffer != nullptr,
                    "If a context is provided, a command buffer must be provided too.");

        
    }
    auto loadMaterial(const MaterialLoadOptions& options) noexcept
        -> tl::expected<Material, AssetLoadError>
    {
        if (!std::filesystem::exists(options.path))
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        if (options.path.extension() != MATERIAL_EXTENSION)
        {
            return tl::make_unexpected(FileFormatError {});
        }

        std::ifstream file(options.path, std::ios::binary);

        if (!file.is_open())
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        Material material;
        cereal::BinaryInputArchive archive(file);
        archive(material);

        return material;
    }

    auto loadMesh(const MeshLoadOptions& options) noexcept -> tl::expected<Mesh, AssetLoadError>
    {
        if (!std::filesystem::exists(options.path))
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        if (options.path.extension() != MESH_EXTENSION)
        {
            return tl::make_unexpected(FileFormatError {});
        }

        std::ifstream file(options.path, std::ios::binary);

        if (!file.is_open())
        {
            return tl::make_unexpected(FileNotFoundError {});
        }

        Mesh mesh;
        cereal::BinaryInputArchive archive(file);
        archive(mesh);

        if (!options.context)
        {
            return mesh;
        }

        debugAssume(
            options.commandBuffer != nullptr && options.sceneBuffer != nullptr,
            "If a context is provided, a command buffer and a scene buffer must be provided too.");

        for (auto&& lod : mesh.lods)
        {
            std::span<const std::byte> vertexData = std::as_bytes(std::span(lod.vertices));

            lod.details.vertexOffset = options.sceneBuffer->uploadData(
                *options.commandBuffer, vertexData, options.access, options.pipelineStage);
            lod.details.vertexCount = static_cast<uint32_t>(lod.vertices.size());

            std::span<const std::byte> indexData = std::as_bytes(std::span(lod.indices));

            lod.details.indexOffset = options.sceneBuffer->uploadData(
                *options.commandBuffer, indexData, options.access, options.pipelineStage);
            lod.details.indexCount = static_cast<uint32_t>(lod.indices.size());
        }

        return mesh;
    }

}  // namespace exage::Renderer

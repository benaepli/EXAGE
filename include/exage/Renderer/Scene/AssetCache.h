#pragma once

#include <filesystem>

#include "exage/Core/Core.h"
#include "exage/Renderer/Scene/Material.h"
#include "exage/Renderer/Scene/Mesh.h"

namespace exage::Renderer
{
    class EXAGE_EXPORT AssetCache
    {
      public:
        AssetCache() noexcept = default;
        ~AssetCache() = default;

        void addMesh(std::weak_ptr<Mesh> mesh) noexcept;
        void addTexture(std::weak_ptr<Texture> texture) noexcept;
        void addMaterial(Material material) noexcept;


        [[nodiscard]] auto getMesh(const std::filesystem::path& path) noexcept
            -> std::shared_ptr<Mesh>;
        [[nodiscard]] auto getTexture(const std::filesystem::path& path) noexcept
            -> std::shared_ptr<Texture>;
        [[nodiscard]] auto getMaterial(const std::filesystem::path& path) noexcept
            -> Material;

      private:
        std::unordered_map<std::filesystem::path, std::weak_ptr<Mesh>> _meshes;
        std::unordered_map<std::filesystem::path, std::weak_ptr<Texture>> _textures;
        std::unordered_map<std::filesystem::path, Material> _materials;
    };
}  // namespace exage::Renderer

#pragma once

#include <glm/glm.hpp>

#include "exage/Core/Core.h"
#include "exage/Graphics/ResourceManager.h"
#include "exage/Graphics/Texture.h"

#include <filesystem>

namespace exage::Renderer
{
    struct Texture
    {
        std::filesystem::path path;
        std::vector<std::byte> data;
        std::shared_ptr<Graphics::Texture> texture;
        Graphics::TextureID gpuID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(path, data, gpuID);
		}
        \
    };

    struct AlbedoInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(1.0f);
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct NormalInfo
    {
        bool useTexture = false;
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct MetallicInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        float factor = 1.0f;
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct RoughnessInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        float factor = 1.0f;
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct OcclusionInfo
    {
        bool useTexture = false;
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct EmissiveInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(0.0f);
        std::shared_ptr<Graphics::Texture> texture;
    };

    struct Material
    {
        std::filesystem::path path;

        AlbedoInfo albedo;
        NormalInfo normal;
        MetallicInfo metallic;
        RoughnessInfo roughness;
        OcclusionInfo occlusion;
        EmissiveInfo emissive;
    };
}  // namespace exage::Renderer

#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "exage/Core/Core.h"
#include "exage/Graphics/ResourceManager.h"
#include "exage/Graphics/Utils/RAII.h"
#include "exage/Graphics/Texture.h"

namespace exage::Renderer
{
    struct Texture
    {
        std::string path;
        std::vector<std::byte> data;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;
        Graphics::TextureID gpuID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(path, data, gpuID);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(path, data, gpuID);
        }
    };

    struct AlbedoInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(1.0f);
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, color);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, color);
        }
    };

    struct NormalInfo
    {
        bool useTexture = false;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture);
        }
    };

    struct MetallicInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        float factor = 1.0f;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, factor);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, value, factor);
        }
    };

    struct RoughnessInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        float factor = 1.0f;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, factor);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, value, factor);
        }
    };

    struct OcclusionInfo
    {
        bool useTexture = false;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture);
        }
    };

    struct EmissiveInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(0.0f);
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, color);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, color);
        }
    };

    struct Material
    {
        std::string path;

        AlbedoInfo albedo;
        NormalInfo normal;
        MetallicInfo metallic;
        RoughnessInfo roughness;
        OcclusionInfo occlusion;
        EmissiveInfo emissive;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(path, albedo, normal, metallic, roughness, occlusion, emissive);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(path, albedo, normal, metallic, roughness, occlusion, emissive);
        }
    };
}  // namespace exage::Renderer

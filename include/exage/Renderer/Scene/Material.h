#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "exage/Core/Core.h"
#include "exage/Graphics/ResourceManager.h"
#include "exage/Graphics/Texture.h"
#include "exage/Graphics/Utils/RAII.h"
#include "exage/utils/glm.h"

namespace exage::Renderer
{
    struct Texture
    {
        std::string path;
        std::vector<std::byte> data;
        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(path, data);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(path, data);
        }
    };

    struct AlbedoInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(1.0f);
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, color, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, color, texturePath);
        }
    };

    struct NormalInfo
    {
        bool useTexture = false;
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, texturePath);
        }
    };

    struct MetallicInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }
    };

    struct RoughnessInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }
    };

    struct OcclusionInfo
    {
        bool useTexture = false;
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, texturePath);
        }
    };

    struct EmissiveInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(0.0f);
        std::string texturePath;
        std::shared_ptr<Texture> texture;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, color, texturePath);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(useTexture, color, texturePath);
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

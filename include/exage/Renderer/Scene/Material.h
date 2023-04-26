﻿#pragma once

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
        Graphics::Format format;
        Graphics::Texture::Type type;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(path, data, format, type);
        }
    };

    struct GPUTexture
    {
        std::string path;

        std::shared_ptr<Graphics::Texture> texture;
        std::shared_ptr<Graphics::RAII::TextureID> textureID;
    };

    constexpr std::string_view TEXTURE_EXTENSION = ".extex";

    struct AlbedoInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(1.0f);
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, color, texturePath);
        }
    };

    struct NormalInfo
    {
        bool useTexture = false;
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, texturePath);
        }
    };

    struct MetallicInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }
    };

    struct RoughnessInfo
    {
        bool useTexture = false;
        float value = 0.0f;
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, value, texturePath);
        }
    };

    struct OcclusionInfo
    {
        bool useTexture = false;
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(useTexture, texturePath);
        }
    };

    struct EmissiveInfo
    {
        bool useTexture = false;
        glm::vec3 color = glm::vec3(0.0f);
        std::string texturePath;
        Texture* texture = nullptr;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
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
    };

    struct GPUMaterial
    {
        struct Data
        {
            alignas(16) glm::vec3 albedoColor = glm::vec3(1.0f);
            alignas(16) glm::vec3 emissiveColor = glm::vec3(0.0f);

            alignas(4) float metallicValue = 0.0f;
            alignas(4) float roughnessValue = 0.0f;

            alignas(4) bool albedoUseTexture = false;
            alignas(4) bool normalUseTexture = false;
            alignas(4) bool metallicUseTexture = false;
            alignas(4) bool roughnessUseTexture = false;
            alignas(4) bool occlusionUseTexture = false;
            alignas(4) bool emissiveUseTexture = false;
        };

        std::string path;

        GPUTexture albedoTexture;
        GPUTexture emissiveTexture;
        GPUTexture normalTexture;
        GPUTexture metallicTexture;
        GPUTexture roughnessTexture;
        GPUTexture occlusionTexture;

        std::shared_ptr<Graphics::Buffer> buffer;
        std::shared_ptr<Graphics::RAII::BufferID> bufferID;
    };

    constexpr std::string_view MATERIAL_EXTENSION = ".exmat";
}  // namespace exage::Renderer
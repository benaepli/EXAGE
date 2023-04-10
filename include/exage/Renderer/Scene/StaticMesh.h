#pragma once

#include <glm/glm.hpp>

#include "cereal/cereal.hpp"
#include "cereal/types/"
#include "cereal/types/vector.hpp"
#include "exage/Core/Core.h"
#include "exage/Renderer/Scene/Material.h"

namespace exage::Renderer
{
    struct MeshDetails
    {
        uint32_t vertexCount;
        uint32_t indexCount;

        size_t vertexOffset;
        size_t indexOffset;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(vertexCount, indexCount, vertexOffset, indexOffset);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(vertexCount, indexCount, vertexOffset, indexOffset);
        }
    };

    struct MeshVertex
    {
        glm::vec3 position {};
        glm::vec3 normal {};
        glm::vec2 uv {};
        glm::vec3 tangent {};
        glm::vec3 bitangent {};

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(position, normal, uv, tangent, bitangent);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(position, normal, uv, tangent, bitangent);
        }
    };

    struct MeshLOD
    {
        MeshDetails details;
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(details, vertices, indices);
        }

        // Deserialization
        template<class Archive>
        void load(Archive& archive)
        {
            archive(details, vertices, indices);
        }
    };

    struct StaticMesh
    {
        std::filesystem::path path;

        std::vector<MeshLOD> lods;
        std::shared_ptr<Material> material;

        // Serialization
        template<class Archive>
        void serialize(Archive& archive)
        {
			archive(path, lods, material);
		}

        // Deserialization
		template<class Archive>
        void load(Archive& archive)
        {
			archive(path, lods, material);
		}
    };
}  // namespace exage::Renderer

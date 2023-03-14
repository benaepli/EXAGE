﻿#pragma once

#include "Graphics/FrameBuffer.h"
#include "Vulkan/VulkanCommandBuffer.h"

namespace exage::Graphics
{
    class EXAGE_EXPORT VulkanFrameBuffer : public FrameBuffer
    {
      public:
        [[nodiscard]] static auto create(VulkanContext& context, glm::uvec2 extent) noexcept
            -> tl::expected<VulkanFrameBuffer, Error>;
        ~VulkanFrameBuffer() override = default;

        EXAGE_DELETE_COPY(VulkanFrameBuffer);
        EXAGE_DEFAULT_MOVE(VulkanFrameBuffer);

        [[nodiscard]] auto getTexture(size_t index) const noexcept
            -> std::shared_ptr<Texture> override;
        [[nodiscard]] auto getDepthStencilTexture() const noexcept
            -> std::shared_ptr<Texture> override;

        [[nodiscard]] auto getTextures() const noexcept
            -> const std::vector<std::shared_ptr<Texture>>& override
        {
            return _textures;
        }

        [[nodiscard]] auto resize(glm::uvec2 extent) noexcept -> std::optional<Error> override;
        [[nodiscard]] auto attachColor(std::shared_ptr<Texture> texture) noexcept
            -> std::optional<Error> override;
        [[nodiscard]] auto attachOrReplaceDepthStencil(std::shared_ptr<Texture> texture) noexcept
            -> std::optional<Error> override;

        EXAGE_VULKAN_DERIVED

      private:
        explicit VulkanFrameBuffer(VulkanContext& context, glm::uvec2 extent) noexcept;

        VulkanContext& _context;
        std::vector<std::shared_ptr<Texture>> _textures;
        std::shared_ptr<Texture> _depthStencilTexture;
    };
}  // namespace exage::Graphics

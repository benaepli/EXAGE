#include "VulkanFrameBuffer.h"

#include "VulkanTexture.h"

namespace exage::Graphics
{
    auto VulkanFrameBuffer::create(VulkanContext& context, glm::uvec2 extent) noexcept
        -> tl::expected<VulkanFrameBuffer, Error>
    {
        VulkanFrameBuffer frameBuffer {context, extent};
        return frameBuffer;
    }

    VulkanFrameBuffer::VulkanFrameBuffer(VulkanContext& context, glm::uvec2 extent) noexcept
        : FrameBuffer(extent)
        , _context(context)
    {
    }
    auto VulkanFrameBuffer::getTexture(size_t index) const noexcept -> std::shared_ptr<Texture>
    {
        if (index >= _textures.size())
        {
            return nullptr;
        }

        return _textures[index];
    }
    auto VulkanFrameBuffer::getDepthStencilTexture() const noexcept -> std::shared_ptr<Texture>
    {
        return _depthStencilTexture;
    }

    auto VulkanFrameBuffer::resize(glm::uvec2 extent) noexcept -> std::optional<Error>
    {
        for (size_t i = 0; i < _textures.size(); ++i)
        {
            TextureCreateInfo createInfo = _textures[i]->getTextureCreateInfo();
            createInfo.extent = glm::uvec3 {extent.x, extent.y, 1};

            tl::expected texture = _context.createTexture(createInfo);
            if (!texture.has_value())
            {
                return texture.error();
            }

            _textures[i] = std::move(texture.value());
        }
    }

    auto VulkanFrameBuffer::attachColor(std::shared_ptr<Texture> texture) noexcept
        -> std::optional<Error>
    {
        if (texture->getExtent() != glm::uvec3 {_extent.x, _extent.y, 1})
        {
            return ErrorCode::eFrameBufferTextureExtentMismatch;
        }

        if (texture->getType() != Texture::Type::e2D)
        {
            return ErrorCode::eFrameBufferTextureType;
        }

        if (texture->getUsage().none(Texture::UsageFlags::eColorAttachment))
        {
            return ErrorCode::eFrameBufferTextureUsage;
        }

        _textures.push_back(texture);
        return std::nullopt;
    }

    auto VulkanFrameBuffer::attachOrReplaceDepthStencil(
        std::shared_ptr<Texture> texture) noexcept
        -> std::optional<Error>
    {
        if (texture->getExtent() != glm::uvec3 {_extent.x, _extent.y, 1})
        {
            return ErrorCode::eFrameBufferTextureExtentMismatch;
        }

        if (texture->getType() != Texture::Type::e2D)
        {
            return ErrorCode::eFrameBufferTextureType;
        }

        if (texture->getUsage().none(Texture::UsageFlags::eDepthStencilAttachment))
        {
            return ErrorCode::eFrameBufferTextureUsage;
        }

        _depthStencilTexture = texture;
        return std::nullopt;
    }
}  // namespace exage::Graphics

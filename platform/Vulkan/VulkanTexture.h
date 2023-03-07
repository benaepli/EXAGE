#pragma once

#include "Core/Core.h"
#include "Graphics/Swapchain.h"
#include "Graphics/Texture.h"
#include "Vulkan/VulkanContext.h"
#include "utils/classes.h"

namespace exage::Graphics
{
    class EXAGE_EXPORT VulkanSampler final : public Sampler
    {
    public:
        static auto create(VulkanContext& context,
                           SamplerCreateInfo& createInfo,
                           uint32_t mipLevelCount) noexcept -> tl::expected<VulkanSampler, Error>;
        ~VulkanSampler() override;

        EXAGE_DEFAULT_MOVE(VulkanSampler);
        EXAGE_DELETE_COPY(VulkanSampler);

        [[nodiscard]] auto getAnisotropy() const noexcept -> Anisotropy override
        {
            return _anisotropy;
        }

        [[nodiscard]] auto getFilter() const noexcept -> Filter override { return _filter; }

        [[nodiscard]] auto getMipmapMode() const noexcept -> MipmapMode override
        {
            return _mipmapMode;
        }

        [[nodiscard]] auto getLodBias() const noexcept -> float override { return _lodBias; }

        EXAGE_VULKAN_DERIVED
    private:
        VulkanSampler(VulkanContext& context, SamplerCreateInfo& createInfo) noexcept;
        [[nodiscard]] auto init(uint32_t mipLevelCount) noexcept -> std::optional<Error>;

        std::reference_wrapper<VulkanContext> _context;

        Anisotropy _anisotropy;
        Filter _filter;
        MipmapMode _mipmapMode;
        float _lodBias;

        vk::Sampler _sampler;
    };

    class EXAGE_EXPORT VulkanTexture final : public Texture
    {
    public:
        [[nodiscard]] static auto create(VulkanContext& context,
                                         TextureCreateInfo& createInfo) noexcept
        -> tl::expected<VulkanTexture, Error>;
        ~VulkanTexture() override;

        EXAGE_DELETE_COPY(VulkanTexture);

        VulkanTexture(VulkanTexture&& old) noexcept;
        auto operator=(VulkanTexture&& old) noexcept -> VulkanTexture&;

        [[nodiscard]] auto getExtent() const noexcept -> TextureExtent override { return _extent; }
        [[nodiscard]] auto getFormat() const noexcept -> Format override { return _format; }
        [[nodiscard]] auto getType() const noexcept -> Type override { return _type; }
        [[nodiscard]] auto getLayout() const noexcept -> Layout override { return _layout; }
        [[nodiscard]] auto getUsage() const noexcept -> Usage override { return _usage; }

        [[nodiscard]] auto getLayerCount() const noexcept -> uint32_t override
        {
            return _layerCount;
        }

        [[nodiscard]] auto getMipLevelCount() const noexcept -> uint32_t override
        {
            return _mipLevelCount;
        }

        [[nodiscard]] auto getSampler() noexcept -> Sampler& override { return *_sampler; }

        [[nodiscard]] auto getSampler() const noexcept -> const Sampler& override
        {
            return *_sampler;
        }

        [[nodiscard]] auto getImage() const noexcept -> vk::Image { return _image; }
        [[nodiscard]] auto getImageView() const noexcept -> vk::ImageView { return _imageView; }

        EXAGE_VULKAN_DERIVED
    private:
        VulkanTexture(VulkanContext& context, TextureCreateInfo& createInfo) noexcept;
        [[nodiscard]] auto init(SamplerCreateInfo& samplerInfo) noexcept -> std::optional<Error>;

        std::reference_wrapper<VulkanContext> _context;

        TextureExtent _extent;
        Format _format;
        Type _type;
        Layout _layout = Layout::eUndefined;
        Usage _usage;

        uint32_t _layerCount;
        uint32_t _mipLevelCount;

        vma::Allocation _allocation;
        vk::Image _image;
        vk::ImageView _imageView;

        std::optional<VulkanSampler> _sampler;
    };
} // namespace exage::Graphics
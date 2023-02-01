#pragma once

#include "Graphics/Context.h"

#define VK_NO_PROTOTYPES
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_STORAGE_SHARED
#define VULKAN_HPP_STORAGE_SHARED_EXPORT

#include <optional>

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>
#include "Graphics/Queue.h"
#include <vulkan-memory-allocator-hpp/vk_mem_alloc.hpp>

namespace exage::Graphics
{
    enum class SurfaceError : uint32_t;


    class EXAGE_EXPORT VulkanContext final : public Context
    {
    public:
        [[nodiscard]] static tl::expected<std::unique_ptr<Context>, Error> create(
            ContextCreateInfo& createInfo) noexcept;
        ~VulkanContext() override;

        EXAGE_DELETE_COPY(VulkanContext);
        EXAGE_DEFAULT_MOVE(VulkanContext);

        void waitIdle() const noexcept override;

        [[nodiscard]] auto createSwapchain(const SwapchainCreateInfo& createInfo) noexcept
        -> tl::expected<std::unique_ptr<Swapchain>, Error> override;

        [[nodiscard]] auto createSurface(
            Window& window) const noexcept -> tl::expected<vk::SurfaceKHR, Error>;

        [[nodiscard]] auto getQueue() noexcept -> Queue& override;
        [[nodiscard]] auto getQueue() const noexcept -> const Queue& override;

        [[nodiscard]] auto getInstance() const noexcept -> vk::Instance;
        [[nodiscard]] auto getPhysicalDevice() const noexcept -> vk::PhysicalDevice;
        [[nodiscard]] auto getDevice() const noexcept -> vk::Device;
        [[nodiscard]] auto getAllocator() const noexcept -> vma::Allocator;

        [[nodiscard]] auto getVulkanBootstrapDevice() const noexcept -> vkb::Device;

        EXAGE_VULKAN_DERIVED

    private:
        VulkanContext() = default;
        auto init(ContextCreateInfo& createInfo) noexcept -> std::optional<Error>;


        vma::Allocator _allocator;
        vkb::Instance _instance;
        vkb::PhysicalDevice _physicalDevice;
        vkb::Device _device;
        Queue* _queue = nullptr; // Not a smart pointer since destruction order is important
    };
} // namespace exage::Graphics

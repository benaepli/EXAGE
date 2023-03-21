#pragma once

#include "Graphics/Buffer.h"

namespace exage::Graphics
{
    struct DynamicFixedBufferCreateInfo
    {
        Context& context;
        Queue& queue;
        size_t size;
    };

    class EXAGE_EXPORT DynamicFixedBuffer
    {
      public:
        explicit DynamicFixedBuffer(const DynamicFixedBufferCreateInfo& createInfo);
        ~DynamicFixedBuffer();
        
        EXAGE_DEFAULT_COPY(DynamicFixedBuffer);
        EXAGE_DEFAULT_MOVE(DynamicFixedBuffer);

        void write(std::span<const std::byte> data, size_t offset) noexcept;
        void read(std::span<std::byte> data, size_t offset) noexcept; // Latency equivalent to frames in flight

        void readBack(CommandBuffer& commandBuffer) noexcept;

        void update(CommandBuffer& commandBuffer) noexcept;

        [[nodiscard]] auto deviceBuffer() const noexcept -> std::shared_ptr<Buffer>;
        [[nodiscard]] auto currentHost() const noexcept -> std::shared_ptr<Buffer>;

      private:
        std::shared_ptr<Buffer> _deviceBuffer;
        std::vector<std::shared_ptr<Buffer>> _hostBuffers;
    };

}  // namespace exage::Graphics

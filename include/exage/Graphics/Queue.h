﻿#pragma once

#include "CommandBuffer.h"
#include "Swapchain.h"
#include "exage/Core/Core.h"
#include "tl/expected.hpp"

namespace exage::Graphics
{
    struct QueueSubmitInfo
    {
        CommandBuffer& commandBuffer;  // Only one command buffer per submit is supported
    };

    struct QueuePresentInfo
    {
        Swapchain& swapchain;
    };

    class EXAGE_EXPORT Queue
    {
      public:
        Queue() noexcept = default;
        virtual ~Queue() = default;
        EXAGE_DELETE_COPY(Queue);
        EXAGE_DEFAULT_MOVE(Queue);

        virtual void startNextFrame() noexcept = 0;
        virtual void submit(QueueSubmitInfo& submitInfo) noexcept = 0;
        [[nodiscard]] virtual auto present(QueuePresentInfo& presentInfo) noexcept
            -> tl::expected<void, Error> = 0;

        virtual void submitTemporary(std::unique_ptr<CommandBuffer> commandBuffer) noexcept = 0;

        [[nodiscard]] virtual auto currentFrame() const noexcept -> size_t = 0;
        [[nodiscard]] virtual auto getFramesInFlight() const noexcept -> size_t = 0;

        EXAGE_BASE_API(API, Queue);
    };
}  // namespace exage::Graphics

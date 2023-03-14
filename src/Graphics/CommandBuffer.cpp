﻿#include "Graphics/CommandBuffer.h"

#include "Graphics/Queue.h"

namespace exage::Graphics
{
    QueueCommandRepo::QueueCommandRepo(QueueCommandRepoCreateInfo& createInfo) noexcept
        : _queue(createInfo.queue)
    {
    }

    auto QueueCommandRepo::init(Context& context) noexcept -> std::optional<Error>
    {
        for (uint32_t i = 0; i < _queue.get().getFramesInFlight(); i++)
        {
            tl::expected commandBuffer = context.createCommandBuffer();
            if (!commandBuffer.has_value())
            {
                return commandBuffer.error();
            }
            _commandBuffers.emplace_back(std::move(commandBuffer.value()));
        }
        return std::nullopt;
    }

    auto QueueCommandRepo::create(QueueCommandRepoCreateInfo& createInfo) noexcept
        -> tl::expected<QueueCommandRepo, Error>

    {
        QueueCommandRepo repo(createInfo);
        std::optional<Error> result = repo.init(createInfo.context);
        if (result)
        {
            return tl::make_unexpected(result.value());
        }
        return repo;
    }

    auto QueueCommandRepo::current() noexcept -> CommandBuffer&
    {
        return *_commandBuffers[_queue.get().currentFrame()];
    }

    auto QueueCommandRepo::current() const noexcept -> const CommandBuffer&
    {
        return *_commandBuffers[_queue.get().currentFrame()];
    }
}  // namespace exage::Graphics

#pragma once

#include <variant>

#include "Core/Core.h"
#include "Graphics/Context.h"
#include "Texture.h"

namespace exage::Graphics
{
    class CommandBuffer;

    // use enum class instead
    enum class PipelineStageFlags : uint32_t
    {
        eTopOfPipe = 1 << 0,
        eDrawIndirect = 1 << 1,
        eVertexInput = 1 << 2,
        eVertexShader = 1 << 3,
        eTessellationControlShader = 1 << 4,
        eTessellationEvaluationShader = 1 << 5,
        eFragmentShader = 1 << 6,
        eEarlyFragmentTests = 1 << 7,
        eLateFragmentTests = 1 << 8,
        eColorAttachmentOutput = 1 << 9,
        eComputeShader = 1 << 10,
        eTransfer = 1 << 11,
        eBottomOfPipe = 1 << 12,
        eHost = 1 << 13,
        eAllGraphics = 1 << 14,
        eAllCommands = 1 << 15
    };
    using PipelineStage = Flags<PipelineStageFlags>;
    EXAGE_ENABLE_FLAGS(PipelineStage);

    enum class AccessFlags : uint32_t
    {
        eIndirectCommandRead = 1 << 0,
        eIndexRead = 1 << 1,
        eVertexAttributeRead = 1 << 2,
        eUniformRead = 1 << 3,
        eInputAttachmentRead = 1 << 4,
        eShaderRead = 1 << 5,
        eShaderWrite = 1 << 6,
        eColorAttachmentRead = 1 << 7,
        eColorAttachmentWrite = 1 << 8,
        eDepthStencilAttachmentRead = 1 << 9,
        eDepthStencilAttachmentWrite = 1 << 10,
        eTransferRead = 1 << 11,
        eTransferWrite = 1 << 12,
        eHostRead = 1 << 13,
        eHostWrite = 1 << 14,
        eMemoryRead = 1 << 15,
        eMemoryWrite = 1 << 16
    };
    using Access = Flags<AccessFlags>;
    EXAGE_ENABLE_FLAGS(Access);

    struct DrawCommand
    {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;
    };

    struct DrawIndexedCommand
    {
        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        int32_t vertexOffset;
        uint32_t firstInstance;
    };

    struct TextureBarrier
    {
        std::shared_ptr<Texture> texture;
        Texture::Layout newLayout;
        PipelineStage srcStage;
        PipelineStage dstStage;
        Access srcAccess;
        Access dstAccess;

        Texture::Layout _oldLayout = Texture::Layout::eUndefined;  // Handled by the command buffer
    };

    struct BlitCommand
    {
        std::shared_ptr<Texture> srcTexture;
        std::shared_ptr<Texture> dstTexture;
        glm::uvec3 srcOffset;
        glm::uvec3 dstOffset;
        uint32_t srcMipLevel;
        uint32_t dstMipLevel;
        uint32_t srcLayer;
        uint32_t dstLayer;
        glm::uvec3 extent;
    };

    struct ClearColorCommand
    {
        std::shared_ptr<Texture> texture;
        glm::vec4 color;
        uint32_t mipLevel;
        uint32_t layer;
    };

    struct UserDefinedCommand
    {
        std::function<void(CommandBuffer&)> commandFunction;
    };

    using GPUCommand = std::
        variant<DrawCommand, DrawIndexedCommand, TextureBarrier, BlitCommand, UserDefinedCommand>;

    using DataDependency =
        std::variant<std::shared_ptr<Texture>>;  // Only required for user defined commands
}  // namespace exage::Graphics

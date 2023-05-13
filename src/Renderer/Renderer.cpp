﻿#include "exage/Renderer/Renderer.h"

#include "entt/core/hashed_string.hpp"
#include "exage/Graphics/Buffer.h"
#include "exage/Graphics/CommandBuffer.h"
#include "exage/Graphics/FrameBuffer.h"
#include "exage/Graphics/Texture.h"
#include "exage/Graphics/Utils/BufferTypes.h"
#include "exage/Renderer/Locations.h"
#include "exage/Renderer/Scene/Camera.h"
#include "exage/Renderer/Scene/Mesh.h"
#include "exage/Renderer/Scene/Transform.h"
#include "exage/Scene/Hierarchy.h"
#include "exage/Scene/Scene.h"

namespace exage::Renderer
{
    namespace
    {

        template<typename T>
        void copyComponent(entt::registry& reg, entt::hashed_string now) noexcept
        {
            auto view = reg.view<T>();
            auto&& renderStorage = reg.storage<T>(now);

            renderStorage.clear();
            renderStorage.insert(view.begin(), view.end(), view.storage().begin());
        }

        template<typename T>
        void copyComponentWithName(entt::registry& reg,
                                   entt::hashed_string previousStorage,
                                   entt::hashed_string currentStorage) noexcept
        {
            auto view = entt::basic_view(reg.storage<T>(currentStorage));
            auto&& renderStorage = reg.storage<T>(previousStorage);

            renderStorage.clear();
            renderStorage.insert(view.begin(), view.end(), view.storage().begin());
        }

    }  // namespace

    Renderer::Renderer(const RendererCreateInfo& createInfo) noexcept
        : _context(createInfo.context)
        , _sceneBuffer(createInfo.sceneBuffer)
        , _extent(createInfo.extent)
        , _geometryRenderer({createInfo.context, createInfo.sceneBuffer, createInfo.extent})
    {
        auto& context = _context.get();

        Graphics::FrameBufferCreateInfo frameBufferCreateInfo {};
        frameBufferCreateInfo.extent = _extent;
        frameBufferCreateInfo.colorAttachments.resize(1);
        frameBufferCreateInfo.colorAttachments[0] = {
            Graphics::Format::eRGBA8,
            Graphics::Texture::UsageFlags::eColorAttachment
                | Graphics::Texture::UsageFlags::eTransferSrc
                | Graphics::Texture::UsageFlags::eSampled
                | Graphics::Texture::UsageFlags::eTransferDst};

        _frameBuffer = context.createFrameBuffer(frameBufferCreateInfo);
    }

    void Renderer::render(Graphics::CommandBuffer& commandBuffer, Scene& scene) noexcept
    {
        auto& context = _context.get();
        auto colorTexture = _frameBuffer->getTexture(0);

        auto camera = getSceneCamera(scene);

        if (!scene.isValid(camera))
        {
            return;
        }

        auto&& transformStorage = scene.registry().storage<Transform3D>(CURRENT_TRANSFORM_3D);
        auto&& cameraStorage = scene.registry().storage<Camera>(CURRENT_CAMERA);

        if (!cameraStorage.contains(camera))
        {
            return;
        }

        auto& cameraComponent = cameraStorage.get(camera);
        auto& cameraTransform = transformStorage.get(camera);

        // View and projection matrices
        auto view = glm::inverse(cameraTransform.globalMatrix);
        auto projection =
            glm::perspective(cameraComponent.fov,
                             static_cast<float>(_extent.x) / static_cast<float>(_extent.y),
                             cameraComponent.near,
                             cameraComponent.far);

        auto&& cameraRenderInfoStorage =
            scene.registry().storage<CameraRenderInfo>(CURRENT_CAMERA_RENDER_INFO);
        if (!cameraRenderInfoStorage.contains(camera))
        {
            cameraRenderInfoStorage.emplace(camera);
        }
        auto& cameraRenderInfo = cameraRenderInfoStorage.get(camera);
        cameraRenderInfo.data.view = view;
        cameraRenderInfo.data.projection = projection;
        cameraRenderInfo.data.viewProjection = projection * view;
        cameraRenderInfo.data.position = cameraTransform.globalPosition;

        if (!cameraRenderInfo.buffer)
        {
            Graphics::DynamicBufferCreateInfo bufferCreateInfo {context};
            bufferCreateInfo.size = sizeof(CameraRenderInfo::Data);
            bufferCreateInfo.cached = false;

            cameraRenderInfo.buffer = Graphics::DynamicFixedBuffer {bufferCreateInfo};
        }

        {
            cameraRenderInfo.buffer->write(std::as_bytes(std::span(&cameraRenderInfo, 1)), 0);
        }

        // Transform buffers
        auto&& transformRenderInfoStorage =
            scene.registry().storage<TransformRenderInfo>(CURRENT_TRANSFORM_RENDER_INFO);
        auto transformView = entt::basic_view(transformStorage);

        for (auto entity : transformView)
        {
            auto& transform = transformView.get<Transform3D>(entity);
            if (!transformRenderInfoStorage.contains(entity))
            {
                transformRenderInfoStorage.emplace(entity);
            }
            auto& transformRenderInfo = transformRenderInfoStorage.get(entity);

            transformRenderInfo.data.model = transform.globalMatrix;
            transformRenderInfo.data.normal = glm::transpose(glm::inverse(transform.globalMatrix));
            transformRenderInfo.data.modelViewProjection =
                cameraRenderInfo.data.viewProjection * transform.globalMatrix;

            if (!transformRenderInfo.buffer)
            {
                Graphics::DynamicBufferCreateInfo bufferCreateInfo {context};
                bufferCreateInfo.size = sizeof(TransformRenderInfo::Data);
                bufferCreateInfo.cached = false;

                transformRenderInfo.buffer = Graphics::DynamicFixedBuffer {bufferCreateInfo};
            }

            transformRenderInfo.buffer->write(std::as_bytes(std::span(&transformRenderInfo, 1)), 0);
        }

        _geometryRenderer.render(commandBuffer, scene);

        commandBuffer.textureBarrier(colorTexture,
                                     Graphics::Texture::Layout::eTransferDst,
                                     Graphics::PipelineStageFlags::eTopOfPipe,
                                     Graphics::PipelineStageFlags::eTransfer,
                                     Graphics::Access {},
                                     Graphics::AccessFlags::eTransferWrite);

        auto albedo = _geometryRenderer.getFrameBuffer().getTexture(2);

        commandBuffer.textureBarrier(albedo,
                                     Graphics::Texture::Layout::eTransferSrc,
                                     Graphics::PipelineStageFlags::eColorAttachmentOutput,
                                     Graphics::PipelineStageFlags::eTransfer,
                                     Graphics::AccessFlags::eColorAttachmentWrite,
                                     Graphics::AccessFlags::eTransferRead);

        commandBuffer.blit(albedo,
                           colorTexture,
                           glm::uvec3 {0},
                           glm::uvec3 {0},
                           0,
                           0,
                           0,
                           0,
                           1,
                           albedo->getExtent(),
                           colorTexture->getExtent());

        commandBuffer.textureBarrier(colorTexture,
                                     Graphics::Texture::Layout::eColorAttachment,
                                     Graphics::PipelineStageFlags::eTransfer,
                                     Graphics::PipelineStageFlags::eColorAttachmentOutput,
                                     Graphics::AccessFlags::eTransferWrite,
                                     Graphics::AccessFlags::eColorAttachmentWrite);

        Graphics::ClearColor clearColor;
        clearColor.clear = false;
        clearColor.color = {0.0f, 0.0f, 0.0f, 1.0f};

        Graphics::ClearDepthStencil clearDepthStencil;
        clearDepthStencil.clear = false;

        commandBuffer.beginRendering(_frameBuffer, {clearColor}, clearDepthStencil);

        commandBuffer.endRendering();
    }

    void Renderer::resize(glm::uvec2 extent) noexcept
    {
        _frameBuffer->resize(extent);
    }

    void copySceneForRenderer(Scene& scene) noexcept
    {
        auto& reg = scene.registry();

        copyComponentWithName<Transform3D>(reg, LAST_TRANSFORM_3D, CURRENT_TRANSFORM_3D);
        copyComponentWithName<GPUMesh>(reg, LAST_GPU_MESH, CURRENT_GPU_MESH);
        copyComponentWithName<Camera>(reg, LAST_CAMERA, CURRENT_CAMERA);

        copyComponent<Transform3D>(reg, CURRENT_TRANSFORM_3D);
        copyComponent<GPUMesh>(reg, CURRENT_GPU_MESH);
        copyComponent<Camera>(reg, CURRENT_CAMERA);
    }

}  // namespace exage::Renderer
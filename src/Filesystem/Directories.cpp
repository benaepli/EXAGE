#include "Filesystem/Directories.h"

namespace exage::Filesystem
{
    namespace
    {
        std::filesystem::path engineAssetDirectory = "assets/exage";
        std::filesystem::path engineShaderDirectory = "shaders/exage";
        std::filesystem::path engineShaderCacheDirectory = "cache/shaders/exage";
    }  // namespace

    void setEngineAssetDirectory(std::filesystem::path path) noexcept
    {
        engineAssetDirectory = std::move(path);
    }

    void setEngineShaderDirectory(std::filesystem::path path) noexcept
    {
        engineShaderDirectory = std::move(path);
    }

    void setEngineShaderCacheDirectory(std::filesystem::path path) noexcept
    {
        engineShaderCacheDirectory = std::move(path);
    }

    const std::filesystem::path& getEngineAssetDirectory() noexcept
    {
        return engineAssetDirectory;
    }

    const std::filesystem::path& getEngineShaderDirectory() noexcept
    {
        return engineShaderDirectory;
    }

    const std::filesystem::path& getEngineShaderCacheDirectory() noexcept
    {
        return engineShaderCacheDirectory;
    }
}  // namespace exage::Filesystem

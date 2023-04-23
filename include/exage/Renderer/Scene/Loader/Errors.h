#pragma once

#include <string>

namespace exage::Renderer
{
    struct FileNotFoundError
    {
    };

    struct FileFormatError
    {
    };

    struct SaveDirectoryError
    {
    };

    using AssetImportError = std::variant<FileNotFoundError, FileFormatError, SaveDirectoryError>;
    using AssetLoadError = std::variant<FileNotFoundError, FileFormatError>;
}  // namespace exage::Renderer

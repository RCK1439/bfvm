#pragma once

#include <string_view>

namespace bfl
{
    struct SourcePosition
    {
        std::size_t Line;
        std::size_t Column;
    };

    extern SourcePosition g_Position;

    void SetProgramName(std::string_view filepath) noexcept;

    void LogInfo(std::string_view fmt, ...) noexcept;
    void LogWarn(std::string_view fmt, ...) noexcept;
    void LogError(std::string_view fmt, ...) noexcept;
    void LogCritical(std::string_view fmt, ...) noexcept;
    
    void LogErrorPosition(std::string_view fmt, ...) noexcept;
} // namespace bflog

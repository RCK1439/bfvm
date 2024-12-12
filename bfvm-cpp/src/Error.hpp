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

    void SetProgramName(std::string_view filepath);

    void LogInfo(std::string_view fmt, ...);
    void LogWarn(std::string_view fmt, ...);
    void LogError(std::string_view fmt, ...);
    void LogFatal(std::string_view fmt, ...);
    
    void LogErrorPosition(std::string_view fmt, ...);
}

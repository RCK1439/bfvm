#include "Error.hpp"

#include <cstdarg>
#include <iostream>
#include <ostream>
#include <string>
#include <print>

namespace bfl
{
    constexpr std::string_view TITLE = "\033[1;37mbfvm: \033[m";

    enum class Level
    {
        INFO = 0,
        WARN,
        ERR,
        CRITICAL
    };

    SourcePosition g_Position;
    static std::string s_ProgName;

    static void PrintFormatted(std::ostream& stream, Level level, std::string_view fmt, va_list args);
    static std::string_view GetLogLevelString(Level level);

    void SetProgramName(std::string_view filepath)
    {
        s_ProgName = filepath;
        const std::size_t pos = s_ProgName.find_last_of('/');

        s_ProgName = s_ProgName.substr(pos + 1);
    }

    void LogInfo(std::string_view fmt, ...)
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cout, Level::INFO, fmt, args);
        va_end(args);
    }

    void LogWarn(std::string_view fmt, ...)
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cout, Level::WARN, fmt, args);
        va_end(args);
    }

    void LogError(std::string_view fmt, ...)
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cerr, Level::ERR, fmt, args);
        va_end(args);

        std::exit(EXIT_FAILURE);
    }

    void LogFatal(std::string_view fmt, ...)
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cerr, Level::CRITICAL, fmt, args);
        va_end(args);

        std::exit(EXIT_FAILURE);
    }

    void LogErrorPosition(std::string_view fmt, ...)
    {
        constexpr std::size_t BUF_SIZE = 512;

        va_list args;
        const std::string_view pre = GetLogLevelString(Level::ERR);
        char buffer[BUF_SIZE];

        std::print(std::cerr, "{}\033[1;37m{}:{}:{}:\033[m {}", TITLE, s_ProgName, g_Position.Line, g_Position.Column, pre);

        va_start(args, fmt);
        std::vsnprintf(buffer, BUF_SIZE, fmt.data(), args);
        va_end(args);

        std::println(std::cerr, "{}", buffer);
        std::exit(EXIT_FAILURE);
    }

    static void PrintFormatted(std::ostream& stream, Level level, std::string_view fmt, va_list args)
    {
        constexpr std::size_t BUF_SIZE = 512;

        const std::string_view pre = GetLogLevelString(level);
        char buffer[BUF_SIZE];

        stream.flush();
        std::print("{}{}", TITLE, pre);

        std::vsnprintf(buffer, BUF_SIZE, fmt.data(), args);
        std::println("{}", buffer);
    }

    inline static std::string_view GetLogLevelString(Level level)
    {
        const std::string_view str[] = 
        {
            "\033[1;32minfo: \033[m",
            "\033[1;33mwarning: \033[m",
            "\033[1;31merror: \033[m",
            "\033[1;31mfatal error: \033[m" 
        };

        return str[static_cast<std::size_t>(level)];
    }
}

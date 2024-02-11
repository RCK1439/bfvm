/**
 * @file   Error.cpp
 * @brief  Implementation of the BFVM logging system.
 * @author Ruan C. Keet
 * @date   2023-11-16
 */

#include "Error.hpp"
#include "Lexer.hpp"
#include "Platform.hpp"

#include <cstdarg>
#include <iostream>
#include <string>

#ifdef BFVM_PLATFORM_WINDOWS
#   include <Windows.h>
#   ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#       define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004 // In case the Windows SDK is too old.
#   endif
#   define DELIM '\\'
#   define POS_FMT "%s\033[1;37m%s:%llu:%llu:\033[m %s"
#else
#   define DELIM '/'
#   define POS_FMT "%s\033[1;37m%s:%lu:%lu:\033[m %s"
#endif

namespace bflog
{

    /* --- constants ------------------------------------------------------------*/

    constexpr std::string_view TITLE = "\033[1;37mbfvm: \033[m";

    /* --- type definitions -----------------------------------------------------*/

    enum class Level
    {
        NONE = 0,
        INFO,
        WARN,
        ERR,
        CRITICAL
    };

    /* --- global variables -----------------------------------------------------*/

    SourcePosition g_Position;     // The position in the source file.
    static std::string s_ProgName; // The name of the source file.

    /* --- function prototypes --------------------------------------------------*/

    /**
     * Prints the formatted text to the specified output stream with a given
     * logging level.
     *
     * @param[in] stream
     *      The output stream to output the text to.
     * @param[in] level
     *      The logging level to output with.
     * @param[in] fmt
     *      The format of the text.
     * @param[in] args
     *      Any variadic arguments passed.
     */
    static void PrintFormatted(std::ostream& stream, Level level, std::string_view fmt, va_list args) noexcept;

    /**
     * Gets the string representation of the logging level.
     *
     * @param[in] level
     *      The logging level to get the string from.
     *
     * @return
     *      The string corresponding to the logging level.
     */
    static std::string_view GetLogLevelString(Level level) noexcept;

    /* --- error interface ------------------------------------------------------*/

    void InitLogger() noexcept
    {
#ifdef BFVM_PLATFORM_WINDOWS
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    }

    void SetProgramName(std::string_view filepath) noexcept
    {
        s_ProgName = filepath;
        const std::size_t pos = s_ProgName.find_last_of(DELIM);

        s_ProgName = s_ProgName.substr(pos + 1);
    }

    void LogInfo(std::string_view fmt, ...) noexcept
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cout, Level::INFO, fmt, args);
        va_end(args);
    }

    void LogWarn(std::string_view fmt, ...) noexcept
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cout, Level::WARN, fmt, args);
        va_end(args);
    }

    void LogError(std::string_view fmt, ...) noexcept
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cerr, Level::ERR, fmt, args);
        va_end(args);

        std::exit(EXIT_FAILURE);
    }

    void LogCritical(std::string_view fmt, ...) noexcept
    {
        va_list args;

        va_start(args, fmt);
        PrintFormatted(std::cerr, Level::CRITICAL, fmt, args);
        va_end(args);

        std::exit(EXIT_FAILURE);
    }

    void LogErrorPosition(std::string_view fmt, ...) noexcept
    {
        constexpr std::size_t BUF_SIZE = 512;

        va_list args;
        std::string_view pre = GetLogLevelString(Level::ERR);
        char buffer[BUF_SIZE];

        std::snprintf(buffer, BUF_SIZE, POS_FMT, TITLE.data(), s_ProgName.data(), g_Position.line,
            g_Position.column, pre.data());
        std::cerr << buffer;

        va_start(args, fmt);
        std::vsnprintf(buffer, BUF_SIZE, fmt.data(), args);
        va_end(args);

        std::cerr << buffer << '\n';
        std::exit(EXIT_FAILURE);
    }

    /* --- utility functions ----------------------------------------------------*/

    static void PrintFormatted(std::ostream& stream, Level level, std::string_view fmt, va_list args) noexcept
    {
        constexpr std::size_t BUF_SIZE = 512;

        const std::string_view pre = GetLogLevelString(level);
        char buffer[BUF_SIZE];

        stream.flush();
        stream << TITLE.data() << pre.data();

        std::vsnprintf(buffer, BUF_SIZE, fmt.data(), args);
        stream << buffer << '\n';
    }

    inline static std::string_view GetLogLevelString(Level level) noexcept
    {
        const std::string_view str[] = 
        {
            "",
            "\033[1;32minfo: \033[m",
            "\033[1;33mwarning: \033[m",
            "\033[1;31merror: \033[m",
            "\033[1;31mfatal error: \033[m" 
        };

        return str[static_cast<std::size_t>(level)];
    }

} // namespace bflog

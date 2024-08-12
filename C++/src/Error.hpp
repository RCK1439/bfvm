#pragma once

#include <string_view>

namespace bfl
{

    /* --- type definitions -------------------------------------------------*/

    /**
     * Represents a position in the source file.
     */
    struct SourcePosition
    {
        std::size_t Line;
        std::size_t Column;
    };

    /* --- external variables -----------------------------------------------*/

    extern SourcePosition g_Position;

    /* --- error interface --------------------------------------------------*/

    /**
     * Sets the name of the program.
     *
     * @param[in] filepath
     *      The path to the Brainfuck source file.
     */
    void SetProgramName(std::string_view filepath) noexcept;

    /**
     * Logs only the given string in the format with an info prefix.
     *
     * @param[in] fmt
     *      The format of the text.
     */
    void LogInfo(std::string_view fmt, ...) noexcept;

    /**
     * Logs only the given string in the format with a warning prefix.
     *
     * @param[in] fmt
     *      The format of the text.
     */
    void LogWarn(std::string_view fmt, ...) noexcept;

    /**
     * Logs the string with the format with an error prefix.
     * This also terminates the virtual machine.
     *
     * @param[in] fmt
     *      The format of the text.
     */
    void LogError(std::string_view fmt, ...) noexcept;

    /**
     * Logs the string with the format with a fatal error prefix.
     * This also terminates the virtual machine.
     *
     * @param[in] fmt
     *      The format of the text.
     */
    void LogCritical(std::string_view fmt, ...) noexcept;

    /**
     * Logs the string with the format with an error prefix.
     * This also logs the position in the source file where the error occured.
     * Then it finally terminates the virtual machine.
     *
     * @param[in] fmt
     *      The format of the text.
     */
    void LogErrorPosition(std::string_view fmt, ...) noexcept;
} // namespace bflog

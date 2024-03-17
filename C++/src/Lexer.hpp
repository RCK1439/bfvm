/**
 * @file   Lexer.hpp
 * @brief  Interface for the lexical analyzer of the BFVM compiler.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace bfc
{

    /* --- type definitions -------------------------------------------------*/

    /**
     * The available tokens of the Brainfuck programming language.
     */
    enum class Token : int8_t
    {
        PLUS        = 0x2B, // '+'
        MINUS       = 0x2D, // '-'
        ARROW_LEFT  = 0x3C, // '<'
        ARROW_RIGHT = 0x3E, // '>'
        DOT         = 0x2E, // '.'
        COMMA       = 0x2C, // ','
        BRACE_LEFT  = 0x5B, // '['
        BRACE_RIGHT = 0x5D, // ']'
        END_OF_FILE = -1    // EOF
    };

    /* --- lexer interface --------------------------------------------------*/

    /**
     * Initializes the lexer for token retrieval.
     *
     * @param[in] filepath
     *      The path to the Brainfuck source file.
     */
    void InitLexer(std::string_view filepath) noexcept;

    /**
     * Releases any resources used by the lexer.
     */
    void CloseLexer() noexcept;

    /**
     * Gets the next token available in the source file.
     *
     * @return
     *      The next token in the stream.
     */
    Token GetToken() noexcept;

} // namespace bfc

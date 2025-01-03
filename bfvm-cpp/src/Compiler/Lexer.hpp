#pragma once

#include <cstdint>
#include <fstream>
#include <string_view>

namespace bfc
{
    enum class Token : int8_t
    {
        NONE        = 0x00,
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

    class LexicalAnalyzer final
    {
    public:
        LexicalAnalyzer() = default;
        LexicalAnalyzer(std::string_view filepath);

        Token GetToken();

    private:
        char NextCharacter();

    private:
        std::ifstream m_SourceFile;
    };
}

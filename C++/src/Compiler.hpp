#pragma once

#include "Lexer.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace bfc
{
    enum class OpCode
    {
        ADDB = 0,
        SUBB,
        ADDP,
        SUBP,
        WRITE,
        READ,
        JZ,
        JMP,
        END
    };

    struct ByteCode final
    {
        OpCode Code;
        union
        {
            uint8_t ByteOffset;
            uint16_t PointerOffset;
            std::size_t Line;
        };

        ByteCode() = default;
        ByteCode(OpCode code, std::size_t operand = 0U) :
            Code(code), Line(operand) {}
    };

    class Compiler final
    {
    public: 
        explicit Compiler(std::string_view filePath) noexcept;

        std::vector<ByteCode> Compile() noexcept;

    private:
        void ParseProgram(std::vector<ByteCode>& out) noexcept;
        void ParseAddByte(std::vector<ByteCode>& out) noexcept;
        void ParseSubByte(std::vector<ByteCode>& out) noexcept;
        void ParseAddPtr(std::vector<ByteCode>& out) noexcept;
        void ParseSubPtr(std::vector<ByteCode>& out) noexcept;
        void ParseWrite(std::vector<ByteCode>& out) noexcept;
        void ParseRead(std::vector<ByteCode>& out) noexcept;
        void ParseConditional(std::vector<ByteCode>& out) noexcept;

    private:
        Lexer m_Lexer;
        Token m_CurrentToken;
        std::size_t m_CurrentLine;
    };
} // namespace bfc

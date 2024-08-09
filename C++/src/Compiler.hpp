/**
 * @file   Compiler.hpp
 * @brief  Interface of the compiler for BFVM
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include "Lexer.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace bfc
{
    enum class OpCode
    {
        ADDB,
        SUBB,
        ADDP,
        SUBP,
        WRITE,
        READ,
        JZ,
        JMP,
        END
    };

    struct ByteCode
    {
        OpCode op;
        union
        {
            uint8_t byte_offset;
            uint16_t pointer_offset;
            std::size_t line;
        };

        ByteCode() = default;
        ByteCode(OpCode code, std::size_t operand = 0U) :
            op(code), line(operand) {}
    };

    class Compiler
    {
    public:
        explicit Compiler(std::string_view filepath) noexcept;

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

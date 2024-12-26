#pragma once

#include "Lexer.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace bfc
{
    enum class OpCode : uint8_t
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

    union Operand
    {
        std::size_t Line;
        uint16_t    PointerOffset;
        uint8_t     ByteOffset;
    };

    struct ByteCode final
    {
        Operand Operand;
        OpCode Code;

        ByteCode() = default;
        ByteCode(OpCode code, std::size_t operand = 0U) :
            Operand(operand), Code(code) {}
    };

    class Compiler final
    {
    public:
        Compiler() = default;
        Compiler(std::string_view filepath);

        std::vector<ByteCode> Compile();

    private:
        void ParseProgram(std::vector<ByteCode>& out);
        void ParseAddByte(std::vector<ByteCode>& out);
        void ParseSubByte(std::vector<ByteCode>& out);
        void ParseAddPtr(std::vector<ByteCode>& out);
        void ParseSubPtr(std::vector<ByteCode>& out);
        void ParseWrite(std::vector<ByteCode>& out);
        void ParseRead(std::vector<ByteCode>& out);
        void ParseConditional(std::vector<ByteCode>& out);
        void ParseChain(Token token, OpCode op, std::vector<ByteCode>& out);
    
    private:
        LexicalAnalyzer m_Lexer;
        std::size_t     m_CurrentLine = 0;
        Token           m_CurrentToken = Token::NONE;
    };

} // namespace bfc

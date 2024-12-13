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
            uint8_t     ByteOffset;
            uint16_t    PointerOffset;
            std::size_t Line;
        };

        ByteCode() = default;
        ByteCode(OpCode code, std::size_t operand = 0U) :
            Code(code), Line(operand) {}
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
        Token           m_CurrentToken = Token::NONE;
        std::size_t     m_CurrentLine = 0;
    };

} // namespace bfc

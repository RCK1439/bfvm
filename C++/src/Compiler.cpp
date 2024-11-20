#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"

#include <stack>

namespace bfc
{
    struct BracePosition
    {
        bfl::SourcePosition SrcPosition;
        std::size_t CodePosition;

        BracePosition(bfl::SourcePosition src, std::size_t code) :
            SrcPosition(src), CodePosition(code) {}
    };

    static void ParseProgram(std::vector<ByteCode>& out);
    static void ParseAddByte(std::vector<ByteCode>& out);
    static void ParseSubByte(std::vector<ByteCode>& out);
    static void ParseAddPtr(std::vector<ByteCode>& out);
    static void ParseSubPtr(std::vector<ByteCode>& out);
    static void ParseWrite(std::vector<ByteCode>& out);
    static void ParseRead(std::vector<ByteCode>& out);
    static void ParseConditional(std::vector<ByteCode>& out);
    static void ParseChain(Token token, OpCode op, std::vector<ByteCode>& out);

    static Token s_CurrentToken = Token::NONE;
    static std::size_t s_CurrentLine = 0;

    void Init(std::string_view filepath)
    {
        LexerInit(filepath);
        bfl::SetProgramName(filepath);
    }

    std::vector<ByteCode> Compile()
    {
        std::vector<ByteCode> out;
        ParseProgram(out);

        return out;
    }

    static void ParseProgram(std::vector<ByteCode>& out)
    {
        s_CurrentToken = GetToken();
        while (s_CurrentToken != Token::END_OF_FILE)
        {
            switch (s_CurrentToken)
            {
                case Token::PLUS:
                    ParseAddByte(out);
                    break;
                case Token::MINUS:
                    ParseSubByte(out);
                    break;
                case Token::ARROW_LEFT:
                    ParseSubPtr(out);
                    break;
                case Token::ARROW_RIGHT:
                    ParseAddPtr(out);
                    break;
                case Token::DOT:
                    ParseWrite(out);
                    break;
                case Token::COMMA:
                    ParseRead(out);
                    break;
                case Token::BRACE_LEFT:
                    ParseConditional(out);
                    break;
                default:
                    bfl::LogErrorPosition("invalid token: %d", static_cast<int32_t>(s_CurrentToken));
                    break;
            }
        }

        out.emplace_back(OpCode::END);
    }

    static void ParseAddByte(std::vector<ByteCode>& out)
    {
        ParseChain(Token::PLUS, OpCode::ADDB, out);
    }

    static void ParseSubByte(std::vector<ByteCode>& out)
    {
        ParseChain(Token::MINUS, OpCode::SUBB, out);
    }

    static void ParseAddPtr(std::vector<ByteCode>& out)
    {
        ParseChain(Token::ARROW_RIGHT, OpCode::ADDP, out);
    }

    static void ParseSubPtr(std::vector<ByteCode>& out)
    {
        ParseChain(Token::ARROW_LEFT, OpCode::SUBP, out);
    }

    static void ParseWrite(std::vector<ByteCode>& out)
    {
        out.emplace_back(OpCode::WRITE);
        s_CurrentToken = GetToken();

        s_CurrentLine++;
    }

    static void ParseRead(std::vector<ByteCode>& out)
    {
        out.emplace_back(OpCode::READ);
        s_CurrentToken = GetToken();

        s_CurrentLine++;
    }

    static void ParseConditional(std::vector<ByteCode>& out)
    {
        std::stack<BracePosition, std::vector<BracePosition>> braces;

        braces.emplace(bfl::g_Position, s_CurrentLine++);
        out.emplace_back(OpCode::JZ);

        s_CurrentToken = GetToken();
        while (!braces.empty())
        {
            if (s_CurrentToken == Token::END_OF_FILE)
            {
                bfl::g_Position = braces.top().SrcPosition;
                bfl::LogErrorPosition("no matching ']'");
            }

            switch (s_CurrentToken)
            {
                case Token::PLUS:
                    ParseAddByte(out);
                    break;
                case Token::MINUS:
                    ParseSubByte(out);
                    break;
                case Token::ARROW_LEFT:
                    ParseSubPtr(out);
                    break;
                case Token::ARROW_RIGHT:
                    ParseAddPtr(out);
                    break;
                case Token::DOT:
                    ParseWrite(out);
                    break;
                case Token::COMMA:
                    ParseRead(out);
                    break;
                case Token::BRACE_LEFT:
                {
                    braces.emplace(bfl::g_Position, s_CurrentLine++);
                    out.emplace_back(OpCode::JZ);

                    s_CurrentToken = GetToken();
                } break;
                case Token::BRACE_RIGHT:
                {
                    const std::size_t open = braces.top().CodePosition;

                    out[open].Line = ++s_CurrentLine;
                    out.emplace_back(OpCode::JMP, open);

                    s_CurrentToken = GetToken();
                    braces.pop();
                } break;
                default:
                    bfl::LogErrorPosition("invalid token: %d", static_cast<int32_t>(s_CurrentToken));
                    break;
            }
        }
    }

    static inline void ParseChain(Token token, OpCode op, std::vector<ByteCode>& out)
    {
        ByteCode& code = out.emplace_back(op);

        while (s_CurrentToken == token)
        {
            code.PointerOffset++;
            s_CurrentToken = GetToken();
        }

        s_CurrentLine++;
    }
}

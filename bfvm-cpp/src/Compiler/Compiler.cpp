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

    Compiler::Compiler(std::string_view filepath) :
        m_Lexer(filepath)
    {
        bfl::SetProgramName(filepath);
    }

    std::vector<ByteCode> Compiler::Compile()
    {
        std::vector<ByteCode> out;
        ParseProgram(out);

        return out;
    }

    void Compiler::ParseProgram(std::vector<ByteCode>& out)
    {
        m_CurrentToken = m_Lexer.GetToken();
        while (m_CurrentToken != Token::END_OF_FILE)
        {
            switch (m_CurrentToken)
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
                    bfl::LogErrorPosition("invalid token: %d", static_cast<int32_t>(m_CurrentToken));
                    break;
            }
        }

        out.emplace_back(OpCode::END);
    }

    void Compiler::ParseAddByte(std::vector<ByteCode>& out)
    {
        ParseChain(Token::PLUS, OpCode::ADDB, out);
    }

    void Compiler::ParseSubByte(std::vector<ByteCode>& out)
    {
        ParseChain(Token::MINUS, OpCode::SUBB, out);
    }

    void Compiler::ParseAddPtr(std::vector<ByteCode>& out)
    {
        ParseChain(Token::ARROW_RIGHT, OpCode::ADDP, out);
    }

    void Compiler::ParseSubPtr(std::vector<ByteCode>& out)
    {
        ParseChain(Token::ARROW_LEFT, OpCode::SUBP, out);
    }

    void Compiler::ParseWrite(std::vector<ByteCode>& out)
    {
        out.emplace_back(OpCode::WRITE);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    void Compiler::ParseRead(std::vector<ByteCode>& out)
    {
        out.emplace_back(OpCode::READ);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    void Compiler::ParseConditional(std::vector<ByteCode>& out)
    {
        std::stack<BracePosition, std::vector<BracePosition>> braces;

        braces.emplace(bfl::g_Position, m_CurrentLine++);
        out.emplace_back(OpCode::JZ);

        m_CurrentToken = m_Lexer.GetToken();
        while (!braces.empty())
        {
            if (m_CurrentToken == Token::END_OF_FILE)
            {
                bfl::g_Position = braces.top().SrcPosition;
                bfl::LogErrorPosition("no matching ']'");
            }

            switch (m_CurrentToken)
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
                    braces.emplace(bfl::g_Position, m_CurrentLine++);
                    out.emplace_back(OpCode::JZ);

                    m_CurrentToken = m_Lexer.GetToken();
                } break;
                case Token::BRACE_RIGHT:
                {
                    const std::size_t open = braces.top().CodePosition;

                    out[open].Line = ++m_CurrentLine;
                    out.emplace_back(OpCode::JMP, open);

                    m_CurrentToken = m_Lexer.GetToken();
                    braces.pop();
                } break;
                default:
                    bfl::LogErrorPosition("invalid token: %d", static_cast<int32_t>(m_CurrentToken));
                    break;
            }
        }
    }

    void Compiler::ParseChain(Token token, OpCode op, std::vector<ByteCode>& out)
    {
        ByteCode& code = out.emplace_back(op);

        while (m_CurrentToken == token)
        {
            code.PointerOffset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }
}

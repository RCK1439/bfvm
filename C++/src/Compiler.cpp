#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"

#include <stack>

namespace bfc {

    struct BracePosition
    {
        bfl::SourcePosition SrcPosition;
        std::size_t CodePosition;

        BracePosition(bfl::SourcePosition src, std::size_t code) noexcept :
            SrcPosition(src), CodePosition(code) {}
    };

    Compiler::Compiler(std::string_view filePath) noexcept :
        m_Lexer(filePath), m_CurrentToken(Token::NONE), m_CurrentLine(0)
    {
        bfl::SetProgramName(filePath);
    }

    std::vector<ByteCode> Compiler::Compile() noexcept
    {
        std::vector<ByteCode> out;
        this->ParseProgram(out);

        return out;
    }

    inline void Compiler::ParseProgram(std::vector<ByteCode>& out) noexcept
    {
        m_CurrentToken = m_Lexer.GetToken();
        while (m_CurrentToken != Token::END_OF_FILE)
        {
            switch (m_CurrentToken)
            {
                case Token::PLUS:
                    this->ParseAddByte(out);
                    break;
                case Token::MINUS:
                    this->ParseSubByte(out);
                    break;
                case Token::ARROW_LEFT:
                    this->ParseSubPtr(out);
                    break;
                case Token::ARROW_RIGHT:
                    this->ParseAddPtr(out);
                    break;
                case Token::DOT:
                    this->ParseWrite(out);
                    break;
                case Token::COMMA:
                    this->ParseRead(out);
                    break;
                case Token::BRACE_LEFT:
                    this->ParseConditional(out);
                    break;
                default:
                    bfl::LogErrorPosition("invalid token: %d", static_cast<int32_t>(m_CurrentToken));
                    break;
            }
        }

        out.emplace_back(OpCode::END);
    }

    inline void Compiler::ParseAddByte(std::vector<ByteCode>& out) noexcept
    {
        this->ParseChain(Token::PLUS, OpCode::ADDB, out);
    }

    inline void Compiler::ParseSubByte(std::vector<ByteCode>& out) noexcept
    {
        this->ParseChain(Token::MINUS, OpCode::SUBB, out);
    }

    inline void Compiler::ParseAddPtr(std::vector<ByteCode>& out) noexcept
    {
        this->ParseChain(Token::ARROW_RIGHT, OpCode::ADDP, out);
    }

    inline void Compiler::ParseSubPtr(std::vector<ByteCode>& out) noexcept
    {
        this->ParseChain(Token::ARROW_LEFT, OpCode::SUBP, out);
    }

    inline void Compiler::ParseWrite(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::WRITE);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    inline void Compiler::ParseRead(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::READ);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    inline void Compiler::ParseConditional(std::vector<ByteCode>& out) noexcept
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
                    this->ParseAddByte(out);
                    break;
                case Token::MINUS:
                    this->ParseSubByte(out);
                    break;
                case Token::ARROW_LEFT:
                    this->ParseSubPtr(out);
                    break;
                case Token::ARROW_RIGHT:
                    this->ParseAddPtr(out);
                    break;
                case Token::DOT:
                    this->ParseWrite(out);
                    break;
                case Token::COMMA:
                    this->ParseRead(out);
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

    inline void Compiler::ParseChain(Token token, OpCode op, std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(op);

        while (m_CurrentToken == token)
        {
            code.PointerOffset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }
} // namespace bfc

#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"

#include <stack>

namespace bfc {

    struct BracePosition
    {
        bfl::SourcePosition src;
        std::size_t code;
    };

    Compiler::Compiler(std::string_view filepath) noexcept :
        m_Lexer(filepath),
        m_CurrentToken(Token::NONE),
        m_CurrentLine(0)
    {
        bfl::SetProgramName(filepath);
    }

    std::vector<ByteCode> Compiler::Compile() noexcept
    {
        std::vector<ByteCode> out;
        this->ParseProgram(out);

        return out;
    }

    void Compiler::ParseProgram(std::vector<ByteCode>& out) noexcept
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

    void Compiler::ParseAddByte(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::ADDB);

        while (m_CurrentToken == Token::PLUS)
        {
            code.byte_offset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }

    void Compiler::ParseSubByte(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::SUBB);

        while (m_CurrentToken == Token::MINUS)
        {
            code.byte_offset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }

    void Compiler::ParseAddPtr(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::ADDP);

        while (m_CurrentToken == Token::ARROW_RIGHT)
        {
            code.pointer_offset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }

    void Compiler::ParseSubPtr(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::SUBP);

        while (m_CurrentToken == Token::ARROW_LEFT)
        {
            code.pointer_offset++;
            m_CurrentToken = m_Lexer.GetToken();
        }

        m_CurrentLine++;
    }

    void Compiler::ParseWrite(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::WRITE);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    void Compiler::ParseRead(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::READ);
        m_CurrentToken = m_Lexer.GetToken();

        m_CurrentLine++;
    }

    void Compiler::ParseConditional(std::vector<ByteCode>& out) noexcept
    {
        std::stack<BracePosition, std::vector<BracePosition>> braces;

        braces.push({ bfl::g_Position, m_CurrentLine++ });
        out.emplace_back(OpCode::JZ);

        m_CurrentToken = m_Lexer.GetToken();
        while (!braces.empty())
        {
            if (m_CurrentToken == Token::END_OF_FILE)
            {
                bfl::g_Position = braces.top().src;
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
                    braces.push({ bfl::g_Position, m_CurrentLine++ });
                    out.emplace_back(OpCode::JZ);

                    m_CurrentToken = m_Lexer.GetToken();
                } break;
                case Token::BRACE_RIGHT:
                {
                    const std::size_t open = braces.top().code;

                    out[open].line = ++m_CurrentLine;
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
} // namespace bfc

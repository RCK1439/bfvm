/**
 * @file   Compiler.cpp
 * @brief  Implementation of the compiler for BFVM.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"

#include <stack>

namespace bfc
{

    /* --- type definitions -------------------------------------------------*/

    struct BracePosition
    {
        bfl::SourcePosition src;
        std::size_t code;
    };

    /* --- global variables -------------------------------------------------*/

    static Token s_Token;
    static std::size_t s_CurrentLine{ 0 };

    /* --- parser routines --------------------------------------------------*/

    static void ParseProgram(std::vector<ByteCode>& out) noexcept;
    static void ParseAddByte(std::vector<ByteCode>& out) noexcept;
    static void ParseSubByte(std::vector<ByteCode>& out) noexcept;
    static void ParseAddPtr(std::vector<ByteCode>& out) noexcept;
    static void ParseSubPtr(std::vector<ByteCode>& out) noexcept;
    static void ParseWrite(std::vector<ByteCode>& out) noexcept;
    static void ParseRead(std::vector<ByteCode>& out) noexcept;
    static void ParseConditional(std::vector<ByteCode>& out) noexcept;

    /* --- compiler interface -----------------------------------------------*/

    void Compile(std::string_view filepath,
        std::vector<ByteCode>& out) noexcept
    {
        InitLexer(filepath);
        bfl::SetProgramName(filepath);

        ParseProgram(out);
        CloseLexer();
    }

    /* --- parser routines --------------------------------------------------*/

    static void ParseProgram(std::vector<ByteCode>& out) noexcept
    {
        s_Token = GetToken();
        while (s_Token != Token::END_OF_FILE)
        {
            switch (s_Token)
            {
                case Token::PLUS:        ParseAddByte(out);     break;
                case Token::MINUS:       ParseSubByte(out);     break;
                case Token::ARROW_LEFT:  ParseSubPtr(out);      break;
                case Token::ARROW_RIGHT: ParseAddPtr(out);      break;
                case Token::DOT:         ParseWrite(out);       break;
                case Token::COMMA:       ParseRead(out);        break;
                case Token::BRACE_LEFT:  ParseConditional(out); break;
                default: bfl::LogErrorPosition("invalid token: %d",
                    static_cast<int32_t>(s_Token));
            }
        }

        out.emplace_back(OpCode::END);
    }

    static void ParseAddByte(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::ADDB);

        while (s_Token == Token::PLUS)
        {
            code.bval++;
            s_Token = GetToken();
        }

        s_CurrentLine++;
    }

    static void ParseSubByte(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::SUBB);

        while (s_Token == Token::MINUS)
        {
            code.bval++;
            s_Token = GetToken();
        }

        s_CurrentLine++;
    }

    static void ParseAddPtr(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::ADDP);

        while (s_Token == Token::ARROW_RIGHT)
        {
            code.dval++;
            s_Token = GetToken();
        }

        s_CurrentLine++;
    }

    static void ParseSubPtr(std::vector<ByteCode>& out) noexcept
    {
        ByteCode& code = out.emplace_back(OpCode::SUBP);

        while (s_Token == Token::ARROW_LEFT)
        {
            code.dval++;
            s_Token = GetToken();
        }

        s_CurrentLine++;
    }

    static void ParseWrite(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::WRITE);
        s_Token = GetToken();

        s_CurrentLine++;
    }

    static void ParseRead(std::vector<ByteCode>& out) noexcept
    {
        out.emplace_back(OpCode::READ);
        s_Token = GetToken();

        s_CurrentLine++;
    }

    static void ParseConditional(std::vector<ByteCode>& out) noexcept
    {
        std::stack<BracePosition, std::vector<BracePosition>> braces;

        braces.push({ bfl::g_Position, s_CurrentLine++ });
        out.emplace_back(OpCode::JZ);

        s_Token = GetToken();
        while (!braces.empty())
        {
            if (s_Token == Token::END_OF_FILE)
            {
                bfl::g_Position = braces.top().src;
                bfl::LogErrorPosition("no matching ']'");
            }
            switch (s_Token)
            {
                case Token::PLUS:        ParseAddByte(out); break;
                case Token::MINUS:       ParseSubByte(out); break;
                case Token::ARROW_LEFT:  ParseSubPtr(out);  break;
                case Token::ARROW_RIGHT: ParseAddPtr(out);  break;
                case Token::DOT:         ParseWrite(out);   break;
                case Token::COMMA:       ParseRead(out);    break;
                case Token::BRACE_LEFT:
                {
                    braces.push({ bfl::g_Position, s_CurrentLine++ });
                    out.emplace_back(OpCode::JZ);

                    s_Token = GetToken();
                }
                break;
                case Token::BRACE_RIGHT:
                {
                    const std::size_t open = braces.top().code;

                    out[open].line = ++s_CurrentLine;
                    out.emplace_back(OpCode::JMP, open);

                    s_Token = GetToken();
                    braces.pop();
                }
                break;
                default: bfl::LogErrorPosition("invalid token: %d",
                    static_cast<int32_t>(s_Token));
            }
        }
    }

} // namespace bfc

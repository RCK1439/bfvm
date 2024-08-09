#include "Lexer.hpp"
#include "Error.hpp"

#include <fstream>

#define BF_CMD(ch) (ch == '+' || ch == '-' || ch == '<' ||\
                    ch == '>' || ch == '.' || ch == ',' ||\
                    ch == '[' || ch == ']')

namespace bfc
{
    Lexer::Lexer(std::string_view filepath) noexcept
    {
        bfl::g_Position.line = 1;
        bfl::g_Position.column = 0;

        m_SourceFile.open(filepath.data());
        if (m_SourceFile.fail())
            bfl::LogCritical("could not open file: %s", filepath.data());
    }

    Lexer::~Lexer() noexcept
    {
        m_SourceFile.close();
    }

    Token Lexer::GetToken() noexcept
    {
        char ch;

        do
        {
            ch = NextCharacter();
        } while (!BF_CMD(ch) && ch != EOF);

        return static_cast<Token>(ch);
    }

    char Lexer::NextCharacter() noexcept
    {
        static char last = '\0';
        const char ret = static_cast<char>(m_SourceFile.get());

        if (last == '\n' && ret != EOF)
        {
            bfl::g_Position.line++;
            bfl::g_Position.column = 1;
        }
        else if (ret == EOF)
        {
            return ret;
        }
        else
        {
            bfl::g_Position.column++;
        }

        last = ret;
        return ret;
    }

} // namespace bfc

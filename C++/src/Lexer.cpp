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
        bfl::g_Position.Line = 1;
        bfl::g_Position.Column = 0;

        m_SourceFile.open(filepath.data());
        if (m_SourceFile.fail())
            bfl::LogCritical("could not open file: %s", filepath.data());
    } 

    Token Lexer::GetToken() noexcept
    {
        char ch = this->NextCharacter();
        while (!BF_CMD(ch) && ch != EOF)
            ch = this->NextCharacter();

        return static_cast<Token>(ch);
    }

    char Lexer::NextCharacter() noexcept
    {
        static char last = '\0';
        const char ret = static_cast<char>(m_SourceFile.get());

        if (last == '\n' && ret != EOF)
        {
            bfl::g_Position.Line++;
            bfl::g_Position.Column = 1;
        }
        else if (ret == EOF)
        {
            return ret;
        }
        else
        {
            bfl::g_Position.Column++;
        }

        last = ret;
        return ret;
    }
} // namespace bfc

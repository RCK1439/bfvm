#include "Lexer.hpp"
#include "Error.hpp"

#include <fstream>

#define IS_BF_COMMAND(ch)\
((ch) == '+' || (ch) == '-' || (ch) == '<' ||\
(ch) == '>' || (ch) == '.' || (ch) == ',' ||\
(ch) == '[' || (ch) == ']')

namespace bfc
{
    static char NextCharacter();

    static std::ifstream s_SourceFile;

    void LexerInit(std::string_view filepath)
    {
        bfl::g_Position.Line = 1;
        bfl::g_Position.Column = 0;

        s_SourceFile.open(filepath.data());
        if (s_SourceFile.fail())
            bfl::LogFatal("could not open file: %s", filepath.data());
    }

    Token GetToken()
    {
        char ch = NextCharacter();
        do
        {
            if (IS_BF_COMMAND(ch))
            {
                return static_cast<Token>(ch);
            }
            else if (ch != EOF)
            {
                ch = NextCharacter();
            }
        } while (ch != EOF);

        return static_cast<Token>(ch);
    }

    static char NextCharacter()
    {
        static char last = '\0';
        const char ret = static_cast<char>(s_SourceFile.get());

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
}

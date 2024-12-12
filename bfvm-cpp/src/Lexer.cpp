#include "Lexer.hpp"
#include "Error.hpp"

#include <fstream>

namespace bfc
{
    static char NextCharacter();
    static bool IsBrainfuckCommand(char ch);

    static std::ifstream s_SourceFile;

    void LexerInit(std::string_view filepath)
    {
        bfl::g_Position.Line = 1;
        bfl::g_Position.Column = 0;

        s_SourceFile.open(filepath.data());
        if (s_SourceFile.fail())
            bfl::LogCritical("could not open file: %s", filepath.data());
    }

    Token GetToken()
    {
        char ch = NextCharacter();
        while (!IsBrainfuckCommand(ch) && ch != EOF)
            ch = NextCharacter();

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

    static bool IsBrainfuckCommand(char ch)
    {
        return ch == '+' || ch == '-' || ch == '<' ||
            ch == '>' || ch == '.' || ch == ',' ||
            ch == '[' || ch == ']';
    }
}

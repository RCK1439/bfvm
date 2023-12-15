/**
 * @file   Lexer.cpp
 * @brief  Implementation of the lexical analyzer for BFVM's compiler.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#include "Lexer.hpp"
#include "Error.hpp"

#include <fstream>
#include <iostream>

/* --- helper macros --------------------------------------------------------*/

#define INSTRUCTION(ch) (ch == '+' || ch == '-' || ch == '<' ||\
                         ch == '>' || ch == '.' || ch == ',' ||\
                         ch == '[' || ch == ']')

namespace bfc
{

    /* --- global variables -----------------------------------------------------*/

    static std::ifstream s_Source; // The Brainfuck source file.

    /* --- function prototypes --------------------------------------------------*/

    /**
     * Retrieves the next character from the source file.
     *
     * @return
     *      The character read from the source file.
     */
    static char NextCharacter() noexcept;

    /* --- lexer interface ------------------------------------------------------*/

    void InitLexer(std::string_view filepath) noexcept
    {
        bflog::g_Position.line = 1;
        bflog::g_Position.column = 0;

        s_Source.open(filepath.data());
        if (s_Source.fail())
        {
            bflog::LogCritical("could not open file: %s", filepath.data());
        }
    }

    void CloseLexer() noexcept
    {
        s_Source.close();
    }

    Token GetToken() noexcept
    {
        char ch;

        do
        {
            ch = NextCharacter();
        } while (!INSTRUCTION(ch) && ch != EOF);

        return static_cast<Token>(ch);
    }

    /* --- utility functions ----------------------------------------------------*/

    inline static char NextCharacter() noexcept
    {
        static char last = 0x00;
        char ret = static_cast<char>(s_Source.get());

        if (last == 0x0A && ret != EOF)
        {
            bflog::g_Position.line++;
            bflog::g_Position.column = 1;
        }
        else if (ret == EOF)
        {
            return ret;
        }
        else
        {
            bflog::g_Position.column++;
        }

        last = ret;
        return ret;
    }

} // namespace bfc

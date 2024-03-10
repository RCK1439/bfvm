/**
 * @file   Compiler.hpp
 * @brief  Interface of the compiler for BFVM
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include <string_view>
#include <vector>

namespace bfc
{

    /* --- type definitions -------------------------------------------------*/

    /**
     * The opcodes available in our BFVM assembly language.
     */
    enum class OpCode
    {
        ADDB,  // addb <val>
        SUBB,  // subb <val>
        ADDP,  // addp <val>
        SUBP,  // subp <val>
        WRITE, // write
        READ,  // read
        JZ,    // jz <line>
        JMP,   // jmp <line>
        END    // end
    };

    /**
     * A combination of opcode and operand (This is the actual instructions
     * the VM interprets during execution)
     */
    struct ByteCode
    {
        OpCode op;
        union
        {
            uint8_t bval;
            uint16_t dval;
            std::size_t line;
        };

        // Just a little default constructor :)
        ByteCode() = default;

        /**
         * Constructs the byte code with an opcode and an operand.
         *
         * @param[in] code
         *      The opcode of the byte code.
         * @param[in] operand
         *      The value associated with theopcode.
         */
        ByteCode(OpCode code, std::size_t operand = 0U) :
            op(code), line(operand) {}
    };

    /* --- compiler interface -----------------------------------------------*/

    /**
     * Compiles the Brainfuck source file to byte code that the virtual
     * machine can run.
     *
     * @param[in] filepath
     *      The path to the Brainfuck source file.
     * @param[out] out
     *      The vector in which the compiled byte code will be saved in.
     */
    void Compile(std::string_view filepath,
        std::vector<ByteCode>& out) noexcept;

} // namespace bfc

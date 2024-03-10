/**
 * @file   VirtualMachine.hpp
 * @brief  Interface for BFVM's execution engine.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include "Compiler.hpp"

namespace bfvm
{

    /* --- virtual machine interface ----------------------------------------*/

    /**
     * Initializes the virtual machine and compiles the source file at the
     * filepath to byte code to be interpreted.
     *
     * @param[in] filepath
     *      The path to a Brainfuck source file.
     */
    void Init(std::string_view filepath) noexcept;

    /**
     * Runs the Brainfuck program.
     */
    void Run() noexcept;

} // namespace bfvm

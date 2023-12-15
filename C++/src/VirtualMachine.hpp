/**
 * @file   VirtualMachine.hpp
 * @brief  Interface for BFVM's execution engine.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include "Compiler.hpp"

#include <array>

namespace bfvm
{

    /* --- virtual machine interface --------------------------------------------*/

    /**
     * Initializes the virtual machine and compiles the source file at the
     * filepath to byte code to be interpreted.
     *
     * @param[in] filepath
     *      The path to a Brainfuck source file.
     */
    void InitVM(std::string_view filepath) noexcept;

    /**
     * Executes the Brainfuck program.
     */
    void Execute() noexcept;

} // namespace bfvm

/**
 * @file   EntryPoint.cpp
 * @brief  Brainfuck Virtual Machine
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#include "Error.hpp"
#include "VirtualMachine.hpp"

#include <iostream>

/* --- main routine ---------------------------------------------------------*/

/**
 * The entry point to the virtual machine.
 *
 * @param[in] argc
 *      The amount of arguments passed via the command line.
 * @param[in] argv
 *      The arguments passed via the command line.
 *
 * @return
 *      The exit status of the program.
 */
int main(int argc, char* argv[])
{
    bflog::InitLogger();

    if (argc < 2)
    {
        bflog::LogCritical("no sources");
    }

    bfvm::InitVM(argv[1]);
    bfvm::Execute();

    return EXIT_SUCCESS;
}

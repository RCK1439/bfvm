/**
 * @file   VirtualMachine.cpp
 * @brief  Implementation of the execution engine of BFVM.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#include "VirtualMachine.hpp"
#include "Error.hpp"

#include <chrono>
#include <iostream>

namespace bfvm
{

    /* --- constants ------------------------------------------------------------*/

    static constexpr std::size_t DATA_SIZE = 30000;    // Size of data array.

    /* --- global variables -----------------------------------------------------*/

    static std::array<u8, DATA_SIZE> s_DataArray{ 0 }; // The data array
    static std::vector<bfc::ByteCode> s_ByteCode;      // The byte code to execute
    static u16 s_DataPointer{ 0 };                     // The data pointer
    static std::size_t s_InstructionPointer{ 0 };      // The instruction pointer

    /* --- execution routines ---------------------------------------------------*/

    static void ADDB(u8 val) noexcept;
    static void SUBB(u8 val) noexcept;
    static void ADDP(u16 val) noexcept;
    static void SUBP(u16 val) noexcept;
    static void WRITE() noexcept;
    static void READ() noexcept;
    static void JZ(std::size_t line) noexcept;
    static void JMP(std::size_t line) noexcept;

    /* --- virtual machine interface --------------------------------------------*/

    void InitVM(std::string_view filepath) noexcept
    {
        bfc::Compile(filepath, s_ByteCode);
    }

    void Execute() noexcept
    {
        using namespace bfc;

        const auto start = std::chrono::high_resolution_clock::now();
        while (s_ByteCode[s_InstructionPointer].op != OpCode::END)
        {
            ByteCode& code = s_ByteCode[s_InstructionPointer];

            switch (code.op)
            {
                case OpCode::ADDB:  ADDB(code.bval); break;
                case OpCode::SUBB:  SUBB(code.bval); break;
                case OpCode::ADDP:  ADDP(code.dval); break;
                case OpCode::SUBP:  SUBP(code.dval); break;
                case OpCode::WRITE: WRITE();         break;
                case OpCode::READ:  READ();          break;
                case OpCode::JZ:    JZ(code.line);   break;
                case OpCode::JMP:   JMP(code.line);  break;
                default: bflog::LogError("unknown opcode: %d", static_cast<i32>(code.op));
            }
        }
        const auto end = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<f32> duration = end - start;
        bflog::LogInfo("time elapsed: %.3fms", duration.count() * 1000.0f);
    }

    /* --- execution routines ---------------------------------------------------*/

    inline void ADDB(u8 val) noexcept
    {
        s_DataArray[s_DataPointer] += val;
        s_InstructionPointer++;
    }

    inline void SUBB(u8 val) noexcept
    {
        s_DataArray[s_DataPointer] -= val;
        s_InstructionPointer++;
    }

    inline void ADDP(u16 val) noexcept
    {
        s_DataPointer += val;
        if (s_DataPointer >= DATA_SIZE)
        {
            bflog::LogError("data pointer out of range");
        }

        s_InstructionPointer++;
    }

    inline void SUBP(u16 val) noexcept
    {
        s_DataPointer -= val;
        if (s_DataPointer >= DATA_SIZE)
        {
            bflog::LogError("data pointer out of range");
        }

        s_InstructionPointer++;
    }

    inline void WRITE() noexcept
    {
        std::cout << s_DataArray[s_DataPointer];
        s_InstructionPointer++;
    }

    inline void READ() noexcept
    {
        std::cin >> s_DataArray[s_DataPointer];
        s_InstructionPointer++;
    }

    inline void JZ(std::size_t line) noexcept
    {
        s_InstructionPointer = (s_DataArray[s_DataPointer] != 0x00) ?
            s_InstructionPointer + 1 : line;
    }

    inline void JMP(std::size_t line) noexcept
    {
        s_InstructionPointer = line;
    }

} // namespace bfvm

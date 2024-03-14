/**
 * @file   VirtualMachine.cpp
 * @brief  Implementation of the execution engine of BFVM.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#include "VirtualMachine.hpp"
#include "Error.hpp"

#include <array>
#include <iostream>

namespace bfvm
{

    /* --- constants --------------------------------------------------------*/

    static constexpr std::size_t DATA_SIZE = 30000;

    /* --- global variables -------------------------------------------------*/

    static std::array<uint8_t, DATA_SIZE> s_DataArray{ 0 };
    static std::vector<bfc::ByteCode> s_ByteCode;
    static uint16_t s_DataPointer{ 0 };
    static std::size_t s_InstructionPointer{ 0 };

    /* --- execution routines -----------------------------------------------*/

    static void ADDB(uint8_t val) noexcept;
    static void SUBB(uint8_t val) noexcept;
    static void ADDP(uint16_t val) noexcept;
    static void SUBP(uint16_t val) noexcept;
    static void WRITE() noexcept;
    static void READ() noexcept;
    static void JZ(std::size_t line) noexcept;
    static void JMP(std::size_t line) noexcept;

    /* --- virtual machine interface ----------------------------------------*/

    void Init(std::string_view filepath) noexcept
    {
        bfc::Compile(filepath, s_ByteCode);
    }

    void Run() noexcept
    {
        using namespace bfc;

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
                default: bfl::LogError("unknown opcode: %d",
                    static_cast<int32_t>(code.op));
            }
        }
    }

    /* --- execution routines -----------------------------------------------*/

    inline void ADDB(uint8_t val) noexcept
    {
        s_DataArray[s_DataPointer] += val;
        s_InstructionPointer++;
    }

    inline void SUBB(uint8_t val) noexcept
    {
        s_DataArray[s_DataPointer] -= val;
        s_InstructionPointer++;
    }

    inline void ADDP(uint16_t val) noexcept
    {
        s_DataPointer += val;
        if (s_DataPointer >= DATA_SIZE)
        {
            bfl::LogError("data pointer out of range");
        }

        s_InstructionPointer++;
    }

    inline void SUBP(uint16_t val) noexcept
    {
        s_DataPointer -= val;
        if (s_DataPointer >= DATA_SIZE)
        {
            bfl::LogError("data pointer out of range");
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

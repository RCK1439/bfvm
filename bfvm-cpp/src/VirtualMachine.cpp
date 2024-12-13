#include "VirtualMachine.hpp"
#include "Error.hpp"
#include "Compiler/Compiler.hpp"

#include <array>
#include <iostream>
#include <print>

namespace bfvm
{
    static constexpr std::size_t DATA_SIZE = 30000;

    static void AddByte(uint8_t offset);
    static void SubByte(uint8_t offset);
    static void AddPtr(uint16_t offset);
    static void SubPtr(uint16_t offset);
    static void Write();
    static void Read();
    static void JumpZero(std::size_t line);
    static void Jump(std::size_t line);

    static std::array<uint8_t, DATA_SIZE> s_DataArray{ 0 };
    static std::vector<bfc::ByteCode> s_ByteCode;
    static uint16_t s_DataPointer{ 0 };
    static std::size_t s_InstructionPointer{ 0 };

    void Init(std::string_view filepath)
    {
        bfc::Compiler compiler(filepath);
        s_ByteCode = compiler.Compile();
    }

    int32_t Run()
    {
        while (s_ByteCode[s_InstructionPointer].Code != bfc::OpCode::END)
        {
            bfc::ByteCode& code = s_ByteCode[s_InstructionPointer];
            switch (code.Code)
            {
                case bfc::OpCode::ADDB:
                    AddByte(code.ByteOffset);
                    break;
                case bfc::OpCode::SUBB:
                    SubByte(code.ByteOffset);
                    break;
                case bfc::OpCode::ADDP:
                    AddPtr(code.PointerOffset);
                    break;
                case bfc::OpCode::SUBP:
                    SubPtr(code.PointerOffset);
                    break;
                case bfc::OpCode::WRITE:
                    Write();
                    break;
                case bfc::OpCode::READ:
                    Read();
                    break;
                case bfc::OpCode::JZ:
                    JumpZero(code.Line);
                    break;
                case bfc::OpCode::JMP:
                    Jump(code.Line);
                    break;
                default:
                    bfl::LogError("unknown opcode: %d", static_cast<int32_t>(code.Code));
                    return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }

    static inline void AddByte(uint8_t offset)
    {
        s_DataArray[s_DataPointer] += offset;
        s_InstructionPointer++;
    }

    static inline void SubByte(uint8_t offset)
    {
        s_DataArray[s_DataPointer] -= offset;
        s_InstructionPointer++;
    }

    static inline void AddPtr(uint16_t offset)
    {
        s_DataPointer += offset;
        if (s_DataPointer >= DATA_SIZE)
            bfl::LogError("data pointer out of range");
        
        s_InstructionPointer++;
    }

    static inline void SubPtr(uint16_t offset)
    {
        s_DataPointer -= offset;
        if (s_DataPointer >= DATA_SIZE)
            bfl::LogError("data pointer out of range");
        
        s_InstructionPointer++;
    }

    static inline void Write()
    {
        std::cout << static_cast<char>(s_DataArray[s_DataPointer]);
        s_InstructionPointer++;
    }

    static inline void Read()
    {
        std::cin >> s_DataArray[s_DataPointer];
        s_InstructionPointer++;
    }

    static inline void JumpZero(std::size_t line)
    {
        s_InstructionPointer = (s_DataArray[s_DataPointer] != 0x00) ? s_InstructionPointer + 1 : line;
    }

    static inline void Jump(std::size_t line)
    {
        s_InstructionPointer = line;
    }
}

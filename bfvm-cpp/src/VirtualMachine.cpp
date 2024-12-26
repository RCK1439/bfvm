#include "VirtualMachine.hpp"
#include "Error.hpp"
#include "Compiler/Compiler.hpp"

#include <iostream>
#include <print>

namespace bfvm
{
    VirtualMachine::VirtualMachine(std::string_view filepath)
    {
        bfc::Compiler compiler(filepath);
        m_ByteCode = compiler.Compile();
    }

    int32_t VirtualMachine::Run()
    {
        while (m_ByteCode[m_InstructionPointer].Code != bfc::OpCode::END)
        {
            bfc::ByteCode& code = m_ByteCode[m_InstructionPointer];
            switch (code.Code)
            {
                case bfc::OpCode::ADDB:
                    AddByte(code.Operand.ByteOffset);
                    break;
                case bfc::OpCode::SUBB:
                    SubByte(code.Operand.ByteOffset);
                    break;
                case bfc::OpCode::ADDP:
                    AddPtr(code.Operand.PointerOffset);
                    break;
                case bfc::OpCode::SUBP:
                    SubPtr(code.Operand.PointerOffset);
                    break;
                case bfc::OpCode::WRITE:
                    Write();
                    break;
                case bfc::OpCode::READ:
                    Read();
                    break;
                case bfc::OpCode::JZ:
                    JumpZero(code.Operand.Line);
                    break;
                case bfc::OpCode::JMP:
                    Jump(code.Operand.Line);
                    break;
                default:
                    bfl::LogError("unknown opcode: %d", static_cast<int32_t>(code.Code));
                    return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }

    void VirtualMachine::AddByte(uint8_t offset)
    {
        m_DataArray[m_DataPointer] += offset;
        m_InstructionPointer++;
    }

    void VirtualMachine::SubByte(uint8_t offset)
    {
        m_DataArray[m_DataPointer] -= offset;
        m_InstructionPointer++;
    }

    void VirtualMachine::AddPtr(uint16_t offset)
    {
        m_DataPointer += offset;
        if (m_DataPointer >= DATA_SIZE)
        {
            bfl::LogError("data pointer out of range");
        }
        
        m_InstructionPointer++;
    }

    void VirtualMachine::SubPtr(uint16_t offset)
    {
        m_DataPointer -= offset;
        if (m_DataPointer >= DATA_SIZE)
        {
            bfl::LogError("data pointer out of range");
        }
        
        m_InstructionPointer++;
    }

    void VirtualMachine::Write()
    {
        std::print("{}", static_cast<char>(m_DataArray[m_DataPointer]));
        m_InstructionPointer++;
    }

    void VirtualMachine::Read()
    {
        std::cin >> m_DataArray[m_DataPointer];
        m_InstructionPointer++;
    }

    void VirtualMachine::JumpZero(std::size_t line)
    {
        m_InstructionPointer = (m_DataArray[m_DataPointer] != 0x00) ? m_InstructionPointer + 1 : line;
    }

    void VirtualMachine::Jump(std::size_t line)
    {
        m_InstructionPointer = line;
    }
}

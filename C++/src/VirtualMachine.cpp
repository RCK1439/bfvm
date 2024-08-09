#include "VirtualMachine.hpp"
#include "Compiler.hpp"
#include "Error.hpp"

#include <array>
#include <iostream>

namespace bfvm
{
    VirtualMachine::VirtualMachine(std::string_view filepath) noexcept
    {
        bfc::Compiler compiler(filepath);
        m_ByteCode = compiler.Compile();
    }
        
    void VirtualMachine::Run() noexcept
    {
        while (m_ByteCode[m_InstructionPointer].op != bfc::OpCode::END)
        {
            bfc::ByteCode& code = m_ByteCode[m_InstructionPointer];

            switch (code.op)
            {
                case bfc::OpCode::ADDB:
                    this->AddByte(code.byte_offset);
                    break;
                case bfc::OpCode::SUBB:
                    this->SubByte(code.byte_offset);
                    break;
                case bfc::OpCode::ADDP:
                    this->AddPtr(code.pointer_offset);
                    break;
                case bfc::OpCode::SUBP:
                    this->SubPtr(code.pointer_offset);
                    break;
                case bfc::OpCode::WRITE:
                    this->Write();
                    break;
                case bfc::OpCode::READ:
                    this->Read();
                    break;
                case bfc::OpCode::JZ:
                    this->JumpZero(code.line);
                    break;
                case bfc::OpCode::JMP:
                    this->Jump(code.line);
                    break;
                default:
                    bfl::LogError("unknown opcode: %d", static_cast<int32_t>(code.op));
                    break;
            }
        }
    }

    inline void VirtualMachine::AddByte(uint8_t offset) noexcept
    {
        m_DataArray[m_DataPointer] += offset;
        m_InstructionPointer++;
    }

    inline void VirtualMachine::SubByte(uint8_t offset) noexcept
    {
        m_DataArray[m_DataPointer] -= offset;
        m_InstructionPointer++;
    }

    inline void VirtualMachine::AddPtr(uint16_t offset) noexcept
    {
        m_DataPointer += offset;
        if (m_DataPointer >= DATA_SIZE)
            bfl::LogError("data pointer out of range");
        
        m_InstructionPointer++;
    }

    inline void VirtualMachine::SubPtr(uint16_t offset) noexcept
    {
        m_DataPointer -= offset;
        if (m_DataPointer >= DATA_SIZE)
            bfl::LogError("data pointer out of range");
        
        m_InstructionPointer++;
    }

    inline void VirtualMachine::Write() noexcept
    {
        std::cout << m_DataArray[m_DataPointer];
        m_InstructionPointer++;
    }

    inline void VirtualMachine::Read() noexcept
    {
        std::cin >> m_DataArray[m_DataPointer];
        m_InstructionPointer++;
    }

    inline void VirtualMachine::JumpZero(std::size_t line) noexcept
    {
        m_InstructionPointer = (m_DataArray[m_DataPointer] != 0x00) ?
            m_InstructionPointer + 1 : line;
    }

    inline void VirtualMachine::Jump(std::size_t line) noexcept
    {
        m_InstructionPointer = line;
    }
} // namespace bfvm

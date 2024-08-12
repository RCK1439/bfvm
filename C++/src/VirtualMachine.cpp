#include "VirtualMachine.hpp"
#include "Compiler.hpp"
#include "Error.hpp"

#include <array>
#include <iostream>
#include <print>

namespace bfvm
{
    VirtualMachine::VirtualMachine(std::string_view filepath) noexcept
    {
        bfc::Compiler compiler(filepath);
        m_ByteCode = compiler.Compile();
    }
        
    void VirtualMachine::Run() noexcept
    {
        while (m_ByteCode[m_InstructionPointer].Code != bfc::OpCode::END)
        {
            bfc::ByteCode& code = m_ByteCode[m_InstructionPointer];

            switch (code.Code)
            {
                case bfc::OpCode::ADDB:
                    this->AddByte(code.ByteOffset);
                    break;
                case bfc::OpCode::SUBB:
                    this->SubByte(code.ByteOffset);
                    break;
                case bfc::OpCode::ADDP:
                    this->AddPtr(code.PointerOffset);
                    break;
                case bfc::OpCode::SUBP:
                    this->SubPtr(code.PointerOffset);
                    break;
                case bfc::OpCode::WRITE:
                    this->Write();
                    break;
                case bfc::OpCode::READ:
                    this->Read();
                    break;
                case bfc::OpCode::JZ:
                    this->JumpZero(code.Line);
                    break;
                case bfc::OpCode::JMP:
                    this->Jump(code.Line);
                    break;
                default:
                    bfl::LogError("unknown opcode: %d", static_cast<int32_t>(code.Code));
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
        std::cout << static_cast<char>(m_DataArray[m_DataPointer]);
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

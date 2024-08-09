/**
 * @file   VirtualMachine.hpp
 * @brief  Interface for BFVM's execution engine.
 * @author Ruan C. Keet
 * @date   2023-11-14
 */

#pragma once

#include "Compiler.hpp"

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

namespace bfvm
{
    static constexpr std::size_t DATA_SIZE = 30000;

    class VirtualMachine
    {
    public:
        explicit VirtualMachine(std::string_view filepath) noexcept;
        
        void Run() noexcept;

    private:
        void AddByte(uint8_t offset) noexcept;
        void SubByte(uint8_t offset) noexcept;
        void AddPtr(uint16_t offset) noexcept;
        void SubPtr(uint16_t offset) noexcept;
        void Write() noexcept;
        void Read() noexcept;
        void JumpZero(std::size_t line) noexcept;
        void Jump(std::size_t line) noexcept;

    private:
        std::array<uint8_t, DATA_SIZE> m_DataArray{ 0 };
        std::vector<bfc::ByteCode> m_ByteCode;
        uint16_t m_DataPointer{ 0 };
        std::size_t m_InstructionPointer{ 0 };
    };
}

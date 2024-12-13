#pragma once

#include "Compiler/Compiler.hpp"

#include <array>
#include <string_view>

namespace bfvm
{
    static constexpr std::size_t DATA_SIZE = 30000;

    class VirtualMachine final
    {
    public:
        VirtualMachine(std::string_view filepath);

        int32_t Run();

    private:
        void AddByte(uint8_t offset);
        void SubByte(uint8_t offset);
        void AddPtr(uint16_t offset);
        void SubPtr(uint16_t offset);
        void Write();
        void Read();
        void JumpZero(std::size_t line);
        void Jump(std::size_t line);

    private:
        std::array<uint8_t, DATA_SIZE> m_DataArray{ 0 };
        std::vector<bfc::ByteCode>     m_ByteCode;
        uint16_t                       m_DataPointer{ 0 };
        std::size_t                    m_InstructionPointer{ 0 };
    };
}

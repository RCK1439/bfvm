#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace bfc {
    enum class OpCode
    {
        ADDB = 0,
        SUBB,
        ADDP,
        SUBP,
        WRITE,
        READ,
        JZ,
        JMP,
        END
    };

    struct ByteCode final
    {
        OpCode Code;
        union
        {
            uint8_t ByteOffset;
            uint16_t PointerOffset;
            std::size_t Line;
        };

        ByteCode() = default;
        ByteCode(OpCode code, std::size_t operand = 0U) :
            Code(code), Line(operand) {}
    };

void Init(std::string_view filepath) noexcept;
std::vector<ByteCode> Compile(void) noexcept;

} // namespace bfc

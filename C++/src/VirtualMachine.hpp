#pragma once

#include <cstdint>
#include <string_view>

namespace bfvm
{
    void Init(std::string_view filepath);
    int32_t Run();
}

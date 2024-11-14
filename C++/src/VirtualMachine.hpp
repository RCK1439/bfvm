#pragma once

#include <cstdint>
#include <string_view>

namespace bfvm {

void Init(std::string_view filepath) noexcept;
int32_t Run(void) noexcept;
 
}

cmake_policy(SET CMP0079 NEW)

set(BFC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/bfc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/lexer.c
)

set(BFC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/error.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/core/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/bfc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bfc/lexer.h
)

add_library(bfc STATIC ${BFC_SOURCES} ${BFC_HEADERS})

if(MSVC)
    target_compile_options(bfc PRIVATE /W4 /WX)
else()
    target_compile_options(bfc PRIVATE -Wall -Werror -Wpedantic -Wextra)
endif()

target_include_directories(bfc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(bfc PROPERTIES
    OUTPUT_NAME "bfc"
)
set(BFVM_SOURCES
    core/error.c
    core/memory.c
    vm/bfvm.c
    main.c
)

set(BFVM_HEADERS
    core/error.h
    core/memory.h
    core/platform.h
    core/types.h
    vm/bfvm.h
)

add_executable(bfvm ${BFVM_SOURCES} ${BFVM_HEADERS})

if(MSVC)
    target_compile_options(bfvm PRIVATE /W4 /WX)
else()
    target_compile_options(bfvm PRIVATE -Wall -Werror -Wpedantic -Wextra)
endif()

target_include_directories(bfvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bfvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../bfc)

target_link_libraries(bfvm bfc)

set_target_properties(bfvm PROPERTIES
    OUTPUT_NAME "bfvm"
    VERSION ${bfvm_VERSION_MAJOR}.${bfvm_VERSION_MINOR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

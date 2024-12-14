package bfc

import "core:fmt"

indentation: u64 = 0

debug_start :: proc(name: string) {
    for i: u64 = 0; i < indentation; i += 1 {
        fmt.print(" ");
    }

    fmt.println(name)
    indentation += 2
}

debug_end :: proc(name: string) {
    indentation -= 2

    for i: u64 = 0; i < indentation; i += 1 {
        fmt.print(" ")
    }

    fmt.println(name)
}
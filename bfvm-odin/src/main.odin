package main

import "core:os"

main :: proc() {
    assert(len(os.args) > 1, "usage: <filename>")

    filepath := os.args[1]
    bfvm_invoke(filepath)
}
package main

import "core:os"
import "bfvm"

main :: proc() {
    assert(len(os.args) > 1, "usage: <filename>")

    filepath := os.args[1]
    bfvm.run(filepath)
}
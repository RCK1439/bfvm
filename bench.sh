#!/usr/bin/env bash

function bench_prog() {
    local prog=$1
    local lang=$2

    local total_time=0
    for i in {0..3}; do
        start=$(date +%s)
        $prog tests/mandelbrot.b
        end=$(date +%s)

        elapsed=$((end - start))
        total_time=$((total_time + elapsed))
    done

    avg_time=$((total_time / 100))
    echo "$2: Avg. Time: $avg_time"
}

function compile_all() {
    echo "compiling all programs"

    # C
    cd C || exit
    make release
    cd ..

    echo "compiled C program"

    # C++
    cd C++ || exit
    make release
    cd ..

    echo "compiled C++ program"

    # Rust
    cd rust || exit
    cargo build --release
    cd ..

    echo "compiled Rust program"

    # Java
    cd Java || exit
    javac -d bin src/*.java
    cd ..

    echo "compiled Java program"
}

function bench_all() {
    echo "running benchmark"

    # C
    echo "C"
    bench_prog "./C/bin/bfvm" "C"

    # C++
    echo "C++"
    bench_prog "./C++/bin/bfvm" "C++"

    # Rust
    echo "Rust"
    bench_prog "./rust/target/release/bfvm" "Rust"

    # Java
    echo "Java"
    bench_prog "$(java -cp Java/bin BFVM)" "Java"
}

compile_all
bench_all

# BFVM - Brainf*ck Virtual Machine
BFVM is a virtual machine that runs the [Brainf*ck programming language](https://en.wikipedia.org/wiki/Brainfuck). The reason why I call this a virtual machine is because there is an internal JIT compiler that compiles the Brainf*ck source code to an intermediate assembly code that is then interpreted by an internal machine.

This project was largely an educational venture for myself when it comes to software architecture and using build systems, like CMake, for C/C++ projects.

## Project structure

```sh
bfvm/
├── bfc/ <--------------------- JIT compiler library
│   ├── bfc/
│   │   ├── core/
│   │   │   ├── error.c
│   │   │   ├── error.h
│   │   │   ├── memory.c
│   │   │   ├── memory.h
│   │   │   ├── platform.h
│   │   │   └── types.h
│   │   ├── bfc.c <------------ Compiler implementation
│   │   ├── bfc.h <------------ Compiler interface
│   │   ├── lexer.c
│   │   └── lexer.h
│   ├── CMakeLists.txt
│   └── compile_flags.txt
├── src/
│   ├── core/
│   │   ├── error.c
│   │   ├── error.h
│   │   ├── memory.c
│   │   ├── memory.h
│   │   ├── platform.h
│   │   └── types.h
│   ├── vm/ <------------------ Virtual machine implementation
│   │   ├── bfvm.c
│   │   └── bfvm.h
│   ├── main.c <--------------- Execution starts here
│   └── CMakeLists.txt
├── tests/ <------------------- Basic tests
│   ├── beer.b
│   ├── bitwidth.b
│   ├── broken.b
│   ├── curse.b
│   ├── hello.b
│   ├── mandelbrot.b
│   ├── nested.b
│   ├── sierpinski.b
│   └── welcome.b
├── CMakeLists.txt
├── compile_flags.txt
└── README.md
```

## Building
1. Clone the repository
```sh
git clone https://github.com/RCK1439/bfvm.git
```

2. Generate the CMake project files
```sh
cmake -B build/
```

3. Change to the newly created `build/` directory
```sh
cd build/
```

4. Finally, build the project
```sh
cmake --build .
```

## Running
To run the virtual machine, simply pass a Brainfuck source file as an argument via the CLI

### On Unix
```sh
./bin/bfvm path/to/brainfuck.b
```

### On Windows
```sh
.\bin\bfvm.exe .\path\to\brainfuck.b
```
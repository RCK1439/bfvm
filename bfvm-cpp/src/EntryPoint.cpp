#include "Error.hpp"
#include "VirtualMachine.hpp"

// mandelbrot.b: 6.48s

int main(int argc, char* argv[])
{
    if (argc < 2)
        bfl::LogFatal("no sources");

    bfvm::VirtualMachine vm(argv[1]);
    return vm.Run();
}

#include "Error.hpp"
#include "VirtualMachine.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
        bfl::LogCritical("no sources");

    bfvm::VirtualMachine vm(argv[1]);
    vm.Run();
}

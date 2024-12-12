#include "Error.hpp"
#include "VirtualMachine.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
        bfl::LogFatal("no sources");

    bfvm::Init(argv[1]);
    return bfvm::Run();
}

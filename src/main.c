#include "vm/bfvm.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    BFVirtualMachine *const vm = bfvmInitVirtualMachine(argc, argv);
    if (!vm)
    {
        return EXIT_FAILURE;
    }

    bfvmRunVirtualMachine(vm);
    bfvmCloseVirtualMachine(vm);
    return EXIT_SUCCESS;
}

#ifndef BFVM_H
#define BFVM_H

typedef struct BFVirtualMachine BFVirtualMachine;

BFVirtualMachine *bfvmInitVirtualMachine(int argc, char **argv);
void bfvmCloseVirtualMachine(BFVirtualMachine *vm);

void bfvmRunVirtualMachine(BFVirtualMachine *vm);

#endif /* BFVM_H */

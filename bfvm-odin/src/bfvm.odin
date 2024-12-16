package main

import "core:fmt"
import "core:mem"

import "bfc"

bfvm_invoke :: proc(filepath: string) {
    vm := create_vm(filepath)
    defer close_vm(&vm)
    
    run_vm(&vm)
}

DATA_SIZE :: 30000

Virtual_Machine :: struct {
    data:            [DATA_SIZE]u8,
    data_ptr:        u16,
    instructions:    [dynamic]bfc.Byte_Code,
    instruction_ptr: u64
}

create_vm :: proc(filepath: string) -> Virtual_Machine {
    compiler := bfc.create_compiler(filepath)
    defer bfc.close_compiler(&compiler)
    
    vm := Virtual_Machine{}
    vm.data = [DATA_SIZE]u8{}
    vm.data_ptr = 0
    vm.instructions = bfc.compile(&compiler)
    vm.instruction_ptr = 0

    return vm
}

close_vm :: proc(vm: ^Virtual_Machine) {
    delete(vm.instructions)
}

run_vm :: proc(vm: ^Virtual_Machine) {
    using bfc

    for vm.instructions[vm.instruction_ptr].opcode != Op_Code.END {
        opcode := vm.instructions[vm.instruction_ptr].opcode
        operand := vm.instructions[vm.instruction_ptr].operand;

        #partial switch opcode {
            case .ADDB: addb(vm, operand.(u8))
            case .SUBB: subb(vm, operand.(u8))
            case .ADDP: addp(vm, operand.(u16))
            case .SUBP: subp(vm, operand.(u16))
            case .READ: read(vm)
            case .WRITE: write(vm)
            case .JMP: jmp(vm, operand.(u64))
            case .JZ: jz(vm, operand.(u64))
        }
    }
}

addb :: proc(vm: ^Virtual_Machine, offset: u8) {
    vm.data[vm.data_ptr] += offset
    vm.instruction_ptr += 1
}

subb :: proc(vm: ^Virtual_Machine, offset: u8) {
    vm.data[vm.data_ptr] -= offset
    vm.instruction_ptr += 1
}

addp :: proc(vm: ^Virtual_Machine, offset: u16) {
    vm.data_ptr += offset
    assert(vm.instruction_ptr < DATA_SIZE, "data pointer out of range")

    vm.instruction_ptr += 1
}

subp :: proc(vm: ^Virtual_Machine, offset: u16) {
    vm.data_ptr -= offset
    assert(vm.instruction_ptr < DATA_SIZE, "data pointer out of range")

    vm.instruction_ptr += 1
}

read :: proc(vm: ^Virtual_Machine) {
    assert(false, "not yet implemented")
}

write :: proc(vm: ^Virtual_Machine) {
    fmt.print(rune(vm.data[vm.data_ptr]))
    vm.instruction_ptr += 1
}

jmp :: proc(vm: ^Virtual_Machine, line: u64) {
    vm.instruction_ptr = line
}

jz :: proc(vm: ^Virtual_Machine, line: u64) {
    if vm.data[vm.data_ptr] != 0 {
        vm.instruction_ptr += 1
    } else {
        vm.instruction_ptr = line
    }
}
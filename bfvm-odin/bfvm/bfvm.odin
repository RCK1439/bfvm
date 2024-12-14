package bfvm

import "core:fmt"
import "core:mem"

import "bfc"

run :: proc() {
    vm := create_vm("main.odin")
    defer close_vm(&vm)
    
    run_vm(&vm)
}

DATA_SIZE :: 30000

VirtualMachine :: struct {
    data_ptr: u16,
    data: [DATA_SIZE]u8,
    instructions: [dynamic]bfc.ByteCode,
    instruction_ptr: u64
}

create_vm :: proc(filepath: string) -> VirtualMachine {
    compiler := bfc.create_compiler(filepath)
    defer bfc.close_compiler(&compiler)
    
    vm := VirtualMachine{}
    vm.data_ptr = 0
    vm.data = [DATA_SIZE]u8{}
    vm.instructions = bfc.compile(&compiler)
    vm.instruction_ptr = 0

    return vm
}

close_vm :: proc(vm: ^VirtualMachine) {
    delete(vm.instructions)
}

run_vm :: proc(vm: ^VirtualMachine) {
    for vm.instructions[vm.instruction_ptr].opcode != bfc.OpCode.END {
        opcode := vm.instructions[vm.instruction_ptr].opcode
        operand := vm.instructions[vm.instruction_ptr].operand;

        #partial switch opcode {
            case .ADDB:
            case .SUBB:
            case .ADDP:
            case .SUBP:
            case .READ:
            case .WRITE:
            case .JMP:
            case .JZ:
        }
    }
}

addb :: proc(vm: ^VirtualMachine, offset: u8) {
    vm.data[vm.data_ptr] += offset
    vm.instruction_ptr += 1
}

subb :: proc(vm: ^VirtualMachine, offset: u8) {
    vm.data[vm.data_ptr] -= offset
    vm.instruction_ptr += 1
}

addp :: proc(vm: ^VirtualMachine, offset: u16) {
    vm.data_ptr += offset
    if vm.data_ptr >= DATA_SIZE {
        // TODO: Error here
    }

    vm.instruction_ptr += 1
}

subp :: proc(vm: ^VirtualMachine, offset: u16) {
    vm.data_ptr -= offset
    if vm.data_ptr >= DATA_SIZE {
        // TODO: Error here
    }

    vm.instruction_ptr += 1
}

read :: proc(vm: ^VirtualMachine) {
    // TODO: Figure out how to read from stdin
}

write :: proc(vm: ^VirtualMachine) {
    fmt.printf("%c", vm.data[vm.data_ptr])
}

jmp :: proc(vm: ^VirtualMachine, line: u64) {
    vm.instruction_ptr = line
}

jz :: proc(vm: ^VirtualMachine, line: u64) {
    if vm.data[vm.data_ptr] != 0 {
        vm.instruction_ptr += 1
    } else {
        vm.instruction_ptr = line
    }
}
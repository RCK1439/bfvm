package bfvm

import "core:fmt"
import "core:mem"

import "bfc"

run :: proc(filepath: string) {
    vm := create_vm(filepath)
    defer close_vm(&vm)
    
    run_vm(&vm)
}

DATA_SIZE :: 30000

Virtual_Machine :: struct {
    data_ptr: u16,
    data: [DATA_SIZE]u8,
    instructions: [dynamic]bfc.Byte_Code,
    instruction_ptr: u64
}

create_vm :: proc(filepath: string) -> Virtual_Machine {
    compiler := bfc.create_compiler(filepath)
    defer bfc.close_compiler(&compiler)
    
    vm := Virtual_Machine{}
    vm.data_ptr = 0
    vm.data = [DATA_SIZE]u8{}
    vm.instructions = bfc.compile(&compiler)
    vm.instruction_ptr = 0

    return vm
}

close_vm :: proc(vm: ^Virtual_Machine) {
    delete(vm.instructions)
}

run_vm :: proc(vm: ^Virtual_Machine) {
    for vm.instructions[vm.instruction_ptr].opcode != bfc.Op_Code.End {
        opcode := vm.instructions[vm.instruction_ptr].opcode
        operand := vm.instructions[vm.instruction_ptr].operand;

        #partial switch opcode {
            case .Addb:
                addb(vm, operand.(u8))
                break
            case .Subb:
                subb(vm, operand.(u8))
                break
            case .Addp:
                addp(vm, operand.(u16))
                break
            case .Subp:
                subp(vm, operand.(u16))
                break
            case .Read:
                read(vm)
                break
            case .Write:
                write(vm)
                break
            case .Jmp:
                jmp(vm, operand.(u64))
                break
            case .Jz:
                jz(vm, operand.(u64))
                break
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
    fmt.printf("%c", vm.data[vm.data_ptr])
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
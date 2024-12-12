package main

type VirtualMachine struct {
	data            []uint8
	data_ptr        uint16
	instruction_ptr uintptr
}

func New() VirtualMachine {
	return VirtualMachine{
		data:            nil,
		data_ptr:        0,
		instruction_ptr: 0,
	}
}

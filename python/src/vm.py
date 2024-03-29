
import sys

from bfc import BFCompiler
from bfc import ByteCode
from bfc import OpCode

class VirtualMachine:
    __code: list[ByteCode]
    __instruction_ptr: int
    __data_array: list[int]
    __data_ptr: int

    def __init__(self, filepath: str):
        bfc: BFCompiler = BFCompiler(filepath)

        self.__code = bfc.compile()
        self.__instruction_ptr = 0
        self.__data_array = [0] * 30000
        self.__data_ptr = 0

    def run(self):
        while self.__code[self.__instruction_ptr].op != OpCode.END:
            code: ByteCode = self.__code[self.__instruction_ptr]
            match code.op:
                case OpCode.ADDB: self.__addb(code.val)
                case OpCode.SUBB: self.__subb(code.val)
                case OpCode.ADDP: self.__addp(code.val)
                case OpCode.SUBP: self.__subp(code.val)
                case OpCode.WRITE: self.__write()
                case OpCode.READ: self.__read()
                case OpCode.JZ: self.__jz(code.val)
                case OpCode.JMP: self.__jmp(code.val)
                case _:
                    print("bfvm: error: unknown opcode")
                    exit(1)

    def __addb(self, val: int):
        self.__data_array[self.__data_ptr] += val
        self.__instruction_ptr += 1

    def __subb(self, val: int):
        self.__data_array[self.__data_ptr] -= val
        self.__instruction_ptr += 1

    def __addp(self, offset: int):
        self.__data_ptr += offset
        if self.__data_ptr >= 30000:
            print("bfvm: error: data pointer out of range")
            exit(1)

        self.__instruction_ptr += 1

    def __subp(self, offset: int):
        self.__data_ptr -= offset
        if self.__data_ptr >= 30000:
            print("bfvm: error: data pointer out of range")
            exit(1)

        self.__instruction_ptr += 1

    def __write(self):
        sys.__stdout__.write(self.__data_array[self.__data_ptr])
        self.__instruction_ptr += 1

    def __read(self):
        self.__data_array[self.__data_ptr] = sys.__stdin__.read()
        self.__instruction_ptr += 1

    def __jz(self, line: int):
        if self.__data_array[self.__data_ptr] != 0x00:
            self.__instruction_ptr += 1
        else:
            self.__instruction_ptr = line

    def __jmp(self, line: int):
        self.__instruction_ptr = line
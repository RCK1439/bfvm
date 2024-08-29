import sys

from bfc import compile, ByteCode, OpCode

class VirtualMachine:
    _byte_code: list[ByteCode]
    _instr_ptr: int
    _data: list[int]
    _data_ptr: int

    def __init__(self, filepath: str):
        byte_code = compile(filepath)
        
        self._byte_code = byte_code
        self._instr_ptr = 0
        self._data = [0] * 30_000
        self._data_ptr = 0

    def run(self):
        while self._byte_code[self._instr_ptr].opcode != OpCode.END:
            match self._byte_code[self._instr_ptr].opcode:
                case OpCode.ADDB: self._addb(self._byte_code[self._instr_ptr].value)
                case OpCode.SUBB: self._subb(self._byte_code[self._instr_ptr].value)
                case OpCode.ADDP: self._addp(self._byte_code[self._instr_ptr].value)
                case OpCode.SUBP: self._subp(self._byte_code[self._instr_ptr].value)
                case OpCode.WRITE: self._write()
                case OpCode.READ: self._read()
                case OpCode.JMP: self._jmp(self._byte_code[self._instr_ptr].value)
                case OpCode.JZ: self._jz(self._byte_code[self._instr_ptr].value)
                case _: assert False, f'unexpected opcode {self._byte_code[self._instr_ptr]}'

    def _addb(self, amount: int):
        self._data[self._data_ptr] += amount
        self._instr_ptr += 1

    def _subb(self, amount: int):
        self._data[self._data_ptr] -= amount
        self._instr_ptr += 1

    def _addp(self, offset: int):
        self._data_ptr += offset
        self._instr_ptr += 1

    def _subp(self, offset: int):
        self._data_ptr -= offset
        self._instr_ptr += 1

    def _write(self):
        sys.stdout.write(chr(self._data[self._data_ptr]))
        self._instr_ptr += 1

    def _read(self):
        val = sys.stdin.read(1)
        self._data[self._data_ptr] = ord(val)
        self._instr_ptr += 1

    def _jmp(self, line: int):
        self._instr_ptr = line

    def _jz(self, line: int):
        if self._data[self._data_ptr] == 0:
            self._instr_ptr = line
        else:
            self._instr_ptr += 1

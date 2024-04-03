use crate::bfc::{Compiler, OpCode};
use crate::{log_error, log_fatal};

use std::io::Read;

const DATA_SIZE: usize = 30000usize;

pub struct VirtualMachine {
    data: [u8; DATA_SIZE],
    data_ptr: usize,
    instructions: Vec<OpCode>,
    instr_ptr: usize
}

impl VirtualMachine {
    pub fn init(filepath: &str) -> Self {
        let mut compiler: Compiler = Compiler::from_source(filepath);

        Self {
            data: [0; DATA_SIZE],
            data_ptr: 0usize,
            instructions: compiler.compile(),
            instr_ptr: 0usize
        }
    }

    pub fn run(&mut self) {
        while self.instructions[self.instr_ptr] != OpCode::End {
            match self.instructions[self.instr_ptr] {
                OpCode::AddB(operand) => self.add_byte(operand),
                OpCode::SubB(operand) => self.sub_byte(operand),
                OpCode::AddP(offset) => self.add_ptr(offset),
                OpCode::SubP(offset) => self.sub_ptr(offset),
                OpCode::Write => self.write(),
                OpCode::Read => self.read(),
                OpCode::Jz(line) => self.jmp_zero(line),
                OpCode::Jmp(line) => self.jmp(line),
                _ => {
                    log_fatal!("illegal opcode: {:?}", self.instructions[self.instr_ptr]);
                    std::process::exit(1);
                }
            }
        }
    }

    #[inline]
    fn add_byte(&mut self, operand: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_add(operand);
        self.instr_ptr += 1;
    }

    #[inline]
    fn sub_byte(&mut self, operand: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_sub(operand);
        self.instr_ptr += 1;
    }

    #[inline]
    fn add_ptr(&mut self, offset: usize) {
        self.data_ptr = self.data_ptr.wrapping_add(offset);
        if self.instr_ptr >= DATA_SIZE {
            log_error!("data pointer out of range");
            std::process::exit(1);
        }

        self.instr_ptr += 1;
    }

    #[inline]
    fn sub_ptr(&mut self, offset: usize) {
        self.data_ptr = self.data_ptr.wrapping_sub(offset);
        if self.instr_ptr >= DATA_SIZE {
            log_error!("data pointer out of range");
            std::process::exit(1);
        }

        self.instr_ptr += 1;
    }

    #[inline]
    fn write(&mut self) {
        print!("{}", self.data[self.data_ptr] as char);
        self.instr_ptr += 1;
    }

    #[inline]
    fn read(&mut self) {
        let mut read: [u8; 1] = [0; 1];
        if let Ok(_bytes) = std::io::stdin().read(&mut read) {
            self.data[self.data_ptr] = read[0];
            self.instr_ptr += 1;
        } else {
            log_error!("failed to read byte");
            std::process::exit(1);
        }
    }

    #[inline]
    fn jmp_zero(&mut self, line: usize) {
        if self.data[self.data_ptr] != 0x00 {
            self.instr_ptr += 1;
        } else {
            self.instr_ptr = line;
        }
    }

    #[inline]
    fn jmp(&mut self, line: usize) {
        self.instr_ptr = line;
    }
}

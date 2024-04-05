
mod bfc;
mod error;
mod lexer;

use bfc::{Compiler, OpCode};

use std::io::Read;
use std::process::ExitCode;

const DATA_SIZE: usize = 30000usize;

pub fn run(args: Vec<String>) -> ExitCode {
    if args.len() < 2 {
        log_fatal!("no sources");
        return ExitCode::FAILURE;
    }

    let vm: Result<VirtualMachine, String> = VirtualMachine::init(&args[1]);
    if let Ok(mut vm) = vm {
        if let Err(msg) = vm.run() {
            log_fatal!("{msg}");
            return ExitCode::FAILURE;
        }
    } else if let Err(msg) = vm {
        log_error!("{msg}");
        return ExitCode::FAILURE;
    }

    ExitCode::SUCCESS
}

struct VirtualMachine {
    data: [u8; DATA_SIZE],
    data_ptr: usize,
    instructions: Vec<OpCode>,
    instr_ptr: usize
}

impl VirtualMachine {
    fn init(filepath: &str) -> Result<Self, String> {
        if let Ok(mut compiler) = Compiler::from_source(filepath) {
            Ok(Self {
                data: [0; DATA_SIZE],
                data_ptr: 0usize,
                instructions: compiler.compile()?,
                instr_ptr: 0usize
            })
        } else {
            return Err(format!("failed to open file: {filepath}"));
        }
    }

    fn run(&mut self) -> Result<(), String> {
        while self.instructions[self.instr_ptr] != OpCode::End {
            match self.instructions[self.instr_ptr] {
                OpCode::AddB(operand) => self.add_byte(operand),
                OpCode::SubB(operand) => self.sub_byte(operand),
                OpCode::AddP(offset) => self.add_ptr(offset)?,
                OpCode::SubP(offset) => self.sub_ptr(offset)?,
                OpCode::Write => self.write(),
                OpCode::Read => self.read()?,
                OpCode::Jz(line) => self.jmp_zero(line),
                OpCode::Jmp(line) => self.jmp(line),
                _ => return Err(format!("illegal opcode: {:?}", self.instructions[self.instr_ptr]))
            }
        }

        Ok(())
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
    fn add_ptr(&mut self, offset: usize) -> Result<(), String> {
        self.data_ptr = self.data_ptr.wrapping_add(offset);
        if self.instr_ptr >= DATA_SIZE {
            return Err(String::from("data pointer out of range"));
        }

        self.instr_ptr += 1;
        Ok(())
    }

    #[inline]
    fn sub_ptr(&mut self, offset: usize) -> Result<(), String> {
        self.data_ptr = self.data_ptr.wrapping_sub(offset);
        if self.instr_ptr >= DATA_SIZE {
            return Err(String::from("data pointer out of range"));
        }

        self.instr_ptr += 1;
        Ok(())
    }

    #[inline]
    fn write(&mut self) {
        print!("{}", self.data[self.data_ptr] as char);
        self.instr_ptr += 1;
    }

    #[inline]
    fn read(&mut self) -> Result<(), String> {
        let mut read: [u8; 1] = [0; 1];
        if let Ok(_bytes) = std::io::stdin().read(&mut read) {
            self.data[self.data_ptr] = read[0];
            self.instr_ptr += 1;
        } else {
            return Err(String::from("failed to read byte"));
        }

        Ok(())
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

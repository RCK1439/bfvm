use std::time::Instant;

use crate::bfc::{ByteCode, Compiler, OpCode};

pub struct VirtualMachine {
    data: [u8; 30000],
    data_ptr: usize,
    byte_code: Vec<ByteCode>,
    instruction_ptr: usize
}

impl VirtualMachine {
    pub fn init(filepath: &str) -> VirtualMachine {
        let mut compiler = Compiler::new(filepath);

        return VirtualMachine {
            data: [0; 30000],
            data_ptr: 0,
            byte_code: compiler.compile(),
            instruction_ptr: 0
        };
    }

    pub fn run(&mut self) {
        let time = Instant::now();
        while self.byte_code[self.instruction_ptr].code != OpCode::End {
            let bc = self.byte_code[self.instruction_ptr];
            match bc.code {
                OpCode::AddB => self.add_byte(bc.operand as u8),
                OpCode::SubB => self.sub_byte(bc.operand as u8),
                OpCode::AddP => self.add_ptr(bc.operand),
                OpCode::SubP => self.sub_ptr(bc.operand),
                OpCode::Write => self.write(),
                OpCode::Read => self.read(),
                OpCode::Jz => self.jump_zero(bc.operand),
                OpCode::Jmp => self.jump(bc.operand),
                _ => debug_assert!(false)
            }
        }
        let elapsed = time
            .elapsed()
            .as_secs_f32();

        println!("bfvm: time elapsed: {}ms", elapsed * 1000.0);
    }

    fn add_byte(&mut self, val: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_add(val);
        self.instruction_ptr += 1;
    }

    fn sub_byte(&mut self, val: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_sub(val);
        self.instruction_ptr += 1;
    }

    fn add_ptr(&mut self, val: usize) {
        self.data_ptr = self.data_ptr.wrapping_add(val);

        if self.data_ptr >= 30000 {
            eprintln!("bfvm: error: data pointer out of range");
            std::process::exit(1);
        }
        self.instruction_ptr += 1;
    }

    fn sub_ptr(&mut self, val: usize) {
        self.data_ptr = self.data_ptr.wrapping_sub(val);

        if self.data_ptr >= 30000 {
            eprintln!("bfvm: error: data pointer out of range");
            std::process::exit(1);
        }
        self.instruction_ptr += 1;
    }

    fn write(&mut self) {
        print!("{}", self.data[self.data_ptr] as char);
        self.instruction_ptr += 1;
    }

    fn read(&mut self) {
        // TODO: Read a single byte in from stdin
        self.instruction_ptr += 1;
    }

    fn jump_zero(&mut self, line: usize) {
        if self.data[self.data_ptr] != 0 {
            self.instruction_ptr += 1;
        } else {
            self.instruction_ptr = line;
        }
    }

    fn jump(&mut self, line: usize) {
        self.instruction_ptr = line;
    }
}
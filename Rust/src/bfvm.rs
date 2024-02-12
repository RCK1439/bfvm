use std::time::Instant;

use crate::bfc::{ByteCode, Compiler, OpCode};

/// Representation of the virtual machine data and object.
pub struct VirtualMachine {
    data: [u8; 30000],
    data_ptr: usize,
    byte_code: Vec<ByteCode>,
    instruction_ptr: usize
}

impl VirtualMachine {
    /// Creates a new instance of the `VirtualMachine`.
    /// 
    /// # Arguments
    /// 
    /// `filepath` - The path to a Brainf*ck source file.
    /// 
    /// # Returns
    /// 
    /// A new instance of the virtual machine.
    pub fn new(filepath: &str) -> Self {
        let mut compiler: Compiler = Compiler::new(filepath);

        VirtualMachine {
            data: [0; 30000],
            data_ptr: 0,
            byte_code: compiler.compile(),
            instruction_ptr: 0
        }
    }

    /// Runs the Brainf*ck program via the virtual machine.
    pub fn run(&mut self) {
        let time: Instant = Instant::now();
        while self.byte_code[self.instruction_ptr].code != OpCode::End {
            let bc: ByteCode = self.byte_code[self.instruction_ptr];
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
        let elapsed: f32 = time
            .elapsed()
            .as_secs_f32();

        println!("bfvm: time elapsed: {}ms", elapsed * 1000.0);
    }

    /// Adds `val` to the byte at the current `data_ptr`. 
    #[inline]
    fn add_byte(&mut self, val: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_add(val);
        self.instruction_ptr += 1;
    }

    /// Subtracts `val` from the byte at the current `data_ptr`.
    #[inline]
    fn sub_byte(&mut self, val: u8) {
        self.data[self.data_ptr] = self.data[self.data_ptr].wrapping_sub(val);
        self.instruction_ptr += 1;
    }

    /// Adds `val` to the `data_ptr`
    #[inline]
    fn add_ptr(&mut self, val: usize) {
        self.data_ptr = self.data_ptr.wrapping_add(val);

        if self.data_ptr >= 30000 {
            eprintln!("bfvm: error: data pointer out of range");
            std::process::exit(1);
        }
        self.instruction_ptr += 1;
    }

    /// Subtracts `val` from the `data_ptr`
    #[inline]
    fn sub_ptr(&mut self, val: usize) {
        self.data_ptr = self.data_ptr.wrapping_sub(val);

        if self.data_ptr >= 30000 {
            eprintln!("bfvm: error: data pointer out of range");
            std::process::exit(1);
        }
        self.instruction_ptr += 1;
    }

    /// Writes the byte at `data_ptr` to the `stdout`.
    #[inline]
    fn write(&mut self) {
        print!("{}", self.data[self.data_ptr] as char);
        self.instruction_ptr += 1;
    }

    /// Reads in a byte from the user via `stdin`.
    #[inline]
    fn read(&mut self) {
        // TODO: Read a single byte in from stdin
        self.instruction_ptr += 1;
    }

    /// Jumps to the instruction at `line` when the byte at `data_ptr` is zero.
    #[inline]
    fn jump_zero(&mut self, line: usize) {
        if self.data[self.data_ptr] != 0 {
            self.instruction_ptr += 1;
        } else {
            self.instruction_ptr = line;
        }
    }

    /// Jumps to the instruction at `line`.
    #[inline]
    fn jump(&mut self, line: usize) {
        self.instruction_ptr = line;
    }
}
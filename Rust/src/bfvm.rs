use crate::bfc::OpCode;

use std::io::Read;

/* --- constants ----------------------------------------------------------- */

const DATA_SIZE: usize = 30000_usize; // The maximum size of the data array

/* --- type definitions ---------------------------------------------------- */

/// This represents the actual virtual machine and its "hardware"
pub struct VirtualMachine {
    data: [u8; DATA_SIZE], // The data array
    data_ptr: usize,       // The pointer in the data array
    code: Vec<OpCode>,     // The vector containing the assembly instructions
    instruction_ptr: usize // A pointer to the current instruction in
}

/* --- implementations ----------------------------------------------------- */

impl VirtualMachine {
    /// Creates an instance of the virtual machine from `code` and returns it.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containinig the assembly instructions.
    pub fn from_asm(code: Vec<OpCode>) -> Self {
        VirtualMachine {
            data: [0; DATA_SIZE],
            data_ptr: 0_usize,
            code,
            instruction_ptr: 0_usize
        }
    }

    /// Runs the Brainfuck program given when this instance of the virtual
    /// machine was created.
    pub fn run(&mut self) {
        let mut op: OpCode = self.code[self.instruction_ptr];

        while op != OpCode::End {
            match op {
                OpCode::AddB(operand) => self.add_byte(operand),
                OpCode::SubB(operand) => self.sub_byte(operand),
                OpCode::AddP(offset) => self.add_ptr(offset),
                OpCode::SubP(offset) => self.sub_ptr(offset),
                OpCode::Write => self.write(),
                OpCode::Read => self.read(),
                OpCode::Jz(line) => self.jmp_zero(line),
                OpCode::Jmp(line) => self.jmp(line),
                OpCode::End => {},
            }

            op = self.code[self.instruction_ptr];
        }
    }

    /// Adds `operand` to the byte pointed to by the data pointer.
    /// 
    /// # Parameters
    /// 
    /// - `operand` - The value to add to the byte.
    #[inline]
    fn add_byte(&mut self, operand: u8) {
        let byte: u8 = self.data[self.data_ptr];

        self.data[self.data_ptr] = byte.wrapping_add(operand);
        self.instruction_ptr += 1;
    }

    /// Subtracts `operand` from the byte pointed to by the data pointer.
    /// 
    /// # Parameters
    /// 
    /// - `operand` - The  value to subtract from the byte.
    #[inline]
    fn sub_byte(&mut self, operand: u8) {
        let byte: u8 = self.data[self.data_ptr];
        
        self.data[self.data_ptr] = byte.wrapping_sub(operand);
        self.instruction_ptr += 1;
    }

    /// Adds `offset` to the data pointer, effectively moving it forward.
    /// 
    /// # Parameters
    /// 
    /// - `offset` - The value to add to the data pointer.
    #[inline]
    fn add_ptr(&mut self, offset: usize) {
        self.data_ptr += offset;
        
        if self.instruction_ptr >= DATA_SIZE {
            println!("bfvm: error: data pointer out of range");
            std::process::exit(0);
        }
        self.instruction_ptr += 1;
    }

    /// Subtracts `offset` from the data pointer, effectively moving it
    /// backward.
    /// 
    /// # Parameters
    /// 
    /// - `offset` - The valur to subtract from the data pointer.
    #[inline]
    fn sub_ptr(&mut self, offset: usize) {
        self.data_ptr -= offset;
        
        if self.instruction_ptr >= DATA_SIZE {
            println!("bfvm: error: data pointer out of range");
            std::process::exit(0);
        }
        self.instruction_ptr += 1;
    }

    /// Writes the byte pointed to by the data pointer as a character to
    /// `stdout`.
    #[inline]
    fn write(&mut self) {
        let ch: char = self.data[self.data_ptr] as char;
        print!("{ch}");
        self.instruction_ptr += 1;
    }

    /// Reads in a byte from `stdin` and saves it to the byte pointed to by the
    /// data pointer.
    #[inline]
    fn read(&mut self) {
        let mut byte_buffer: [u8; 1] = [0_u8];
        let res: std::io::Result<usize> = std::io::stdin().read(&mut byte_buffer);
        
        if res.is_err() {
            println!("bfvm: error: failed to read byte");
            std::process::exit(0);
        }
        
        self.instruction_ptr += 1;
    }

    /// Jumps to `line` in the assembly instructions if the byte pointed to by
    /// the data pointer is the value `0`.
    /// 
    /// # Parameters
    /// 
    /// - `line` - The index into the assembly vector to jump to.
    #[inline]
    fn jmp_zero(&mut self, line: usize) {
        if self.data[self.data_ptr] != 0x00 {
            self.instruction_ptr += 1;
        } else {
            self.instruction_ptr = line;
        }
    }

    /// Jumps to `line` in the assembly instructions.
    /// 
    /// # Parameters
    /// 
    /// - `line` - The index into the assembly vector to jump to.
    #[inline]
    fn jmp(&mut self, line: usize) {
        self.instruction_ptr = line;
    }
}
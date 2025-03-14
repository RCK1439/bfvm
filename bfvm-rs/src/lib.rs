mod prelude;
mod bfc;
mod error;

use prelude::*;
use bfc::{Compiler, OpCode};
use error::BFVMError;

use std::io::Read;

// --- public interface to the BFVM -------------------------------------------

/// Runs the Brainfuck Virtual Machine
/// 
/// # Params
/// 
/// - `args` - The arguments passed from the command line
/// 
/// # Errors
/// 
/// - If no source files were provided
/// - If there was an error during compilation
/// - If there was an error during runtime
pub fn run(args: Vec<String>) -> Result<()> {
    if args.len() < 2 {
        return Err(BFVMError::NoSources);
    }
    
    let mut vm = VirtualMachine::new(&args[1])?;
    vm.run()
}

// --- constants --------------------------------------------------------------

/// This is the size of the data array defined by a general Brainfuck standard
const DATA_SIZE: usize = 30000;

// --- virtual machine definition ---------------------------------------------

/// This struct represents the actual virtual machine containing the data
/// array, data pointer, compiled byte code and instruction pointer
struct VirtualMachine {
    /// The data array
    data: [u8; DATA_SIZE],
    /// The data pointer pointing into `data`
    data_ptr: u16,
    /// The compiled byte code instructions to be interpreted
    instructions: Vec<OpCode>,
    /// The instruction pointer pointing into `instructions`
    instr_ptr: usize
}

impl VirtualMachine {
    /// Initializes the virtual machine and returns an instance to the
    /// ready-to-run virtual machine
    /// 
    /// # Params
    /// 
    /// - `filepath` - The path to a Brainfuck source file
    /// 
    /// # Errors
    /// 
    /// - If the source file was invalid
    /// - If there was an error during compilation
    fn new(filepath: &str) -> Result<Self> {
        let mut compiler = Compiler::new(filepath)?;
        
        Ok(Self {
            data: [0; DATA_SIZE],
            data_ptr: 0,
            instructions: compiler.compile()?,
            instr_ptr: 0
        })
    }

    /// Runs the Brainfuck program through the virtual machine
    /// 
    /// # Errors
    /// 
    /// If any errors occur during runtime
    fn run(&mut self) -> Result<()> {
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
                _ => panic!("illegal opcode: {:?}", self.instructions[self.instr_ptr])
            }
        }

        Ok(())
    }

    /// Adds `operand` to the byte pointed to by the data pointer
    /// 
    /// # Params
    /// 
    /// - `operand` - The value to add to the byte
    fn add_byte(&mut self, operand: u8) {
        self.data[self.data_ptr as usize] = self.data[self.data_ptr as usize].wrapping_add(operand);
        self.instr_ptr += 1;
    }

    /// Subtracts `operand` from the byte pointed to by the data pointer
    /// 
    /// # Params
    /// 
    /// - `operand` - The value to subtract from the byte
    fn sub_byte(&mut self, operand: u8) {
        self.data[self.data_ptr as usize] = self.data[self.data_ptr as usize].wrapping_sub(operand);
        self.instr_ptr += 1;
    }

    /// Adds `offset` to the data pointer
    /// 
    /// # Params
    /// 
    /// - `offset` - The value to offset the data pointer by
    /// 
    /// # Errors
    /// 
    /// If the data pointer is larger than `DATA_SIZE` after the addition
    fn add_ptr(&mut self, offset: u16) -> Result<()> {
        self.data_ptr = self.data_ptr.wrapping_add(offset);
        if self.data_ptr as usize >= DATA_SIZE {
            return Err(BFVMError::DataPointerOutOfRange);
        }

        self.instr_ptr += 1;
        Ok(())
    }

    /// Subtracts `offset` from the data pointer
    /// 
    /// # Params
    /// 
    /// - `offset` - The value to offset the data pointer by
    /// 
    /// # Errors
    /// 
    /// If the data pointer is larger than `DATA_SIZE` after the subtraction
    fn sub_ptr(&mut self, offset: u16) -> Result<()> {
        self.data_ptr = self.data_ptr.wrapping_sub(offset);
        if self.data_ptr as usize >= DATA_SIZE {
            return Err(BFVMError::DataPointerOutOfRange);
        }

        self.instr_ptr += 1;
        Ok(())
    }

    /// Writes the byte pointed to by the data pointer to `stdout`
    fn write(&mut self) {
        print!("{}", self.data[self.data_ptr as usize] as char);
        self.instr_ptr += 1;
    }

    /// Reads a byte in from `sdtin` and saves said byte to the address in the
    /// data array pointed by the data pointer
    /// 
    /// # Errors
    /// 
    /// If a byte could not be read from the user
    fn read(&mut self) -> Result<()> {
        let mut read: [u8; 1] = [0; 1];
        std::io::stdin()
            .read(&mut read)
            .map_err(|_| BFVMError::FailedToReadByte)?;

        Ok(())
    }

    /// Jumps to the instruction at `line` if the byte pointed to by the data
    /// pointer is `0`
    /// 
    /// # Params
    /// 
    /// - `line` - The line number of the instruction to jump to
    fn jmp_zero(&mut self, line: usize) {
        if self.data[self.data_ptr as usize] != 0x00 {
            self.instr_ptr += 1;
        } else {
            self.instr_ptr = line;
        }
    }

    /// Jumps to the instruction at `line`
    /// 
    /// # Params
    /// 
    /// - `line` - The line number of the instruction to jump to
    fn jmp(&mut self, line: usize) {
        self.instr_ptr = line;
    }
}

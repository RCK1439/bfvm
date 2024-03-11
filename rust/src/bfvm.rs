use crate::bfc::OpCode;

const DATA_SIZE: usize = 30000;

pub struct VirtualMachine {
    data: [u8; DATA_SIZE],
    data_ptr: usize,
    code: Vec<OpCode>,
    instruction_ptr: usize
}

impl VirtualMachine {
    pub fn from_asm(code: Vec<OpCode>) -> Self {
        VirtualMachine {
            data: [0; DATA_SIZE],
            data_ptr: 0_usize,
            code,
            instruction_ptr: 0_usize
        }
    }

    pub fn run(&mut self) {
        let mut op: &OpCode = &self.code[self.instruction_ptr];

        while *op != OpCode::End {
            match *op {
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

            op = &self.code[self.instruction_ptr];
        }
    }

    #[inline]
    fn add_byte(&mut self, operand: u8) {
        let byte: u8 = self.data[self.data_ptr];

        self.data[self.data_ptr] = byte.wrapping_add(operand);
        self.instruction_ptr += 1;
    }

    #[inline]
    fn sub_byte(&mut self, operand: u8) {
        let byte: u8 = self.data[self.data_ptr];
        
        self.data[self.data_ptr] = byte.wrapping_sub(operand);
        self.instruction_ptr += 1;
    }

    #[inline]
    fn add_ptr(&mut self, offset: usize) {
        self.data_ptr += offset;
        
        if self.instruction_ptr >= DATA_SIZE {
            println!("bfvm: error: data pointer out of range");
            std::process::exit(0);
        }
        self.instruction_ptr += 1;
    }

    #[inline]
    fn sub_ptr(&mut self, offset: usize) {
        self.data_ptr -= offset;
        
        if self.instruction_ptr >= DATA_SIZE {
            println!("bfvm: error: data pointer out of range");
            std::process::exit(0);
        }
        self.instruction_ptr += 1;
    }

    #[inline]
    fn write(&mut self) {
        let ch: char = self.data[self.data_ptr] as char;
        print!("{ch}");
    }

    #[inline]
    fn read(&mut self) {
        todo!();
    }

    #[inline]
    fn jmp_zero(&mut self, line: usize) {
        if self.data[self.data_ptr] != 0x00 {
            self.instruction_ptr += 1;
        } else {
            self.instruction_ptr = line;
        }
    }

    #[inline]
    fn jmp(&mut self, line: usize) {
        self.instruction_ptr = line;
    }
}
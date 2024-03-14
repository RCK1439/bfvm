use crate::{lexer::{Lexer, Token}, log_error};

/* --- type definitions ---------------------------------------------------- */

/// This represents the assembly instructions for the virtual machine.
#[derive(PartialEq, Clone, Copy)]
pub enum OpCode {
    AddB(u8),
    SubB(u8),
    AddP(usize),
    SubP(usize),
    Write,
    Read,
    Jmp(usize),
    Jz(usize),
    End
}

/// This represents the JIT-compiler for the the virtual machine.
pub struct Compiler {
    curr_tok: Option<Token>, // The current token in the source file.
    lexer: Lexer,            // The lexical analyzer.
    curr_ln: usize           // The current line in the assembly code.
}

/* --- implementations ----------------------------------------------------- */

impl Compiler {
    /// Creates and returns the compiler from `filepath`.
    /// 
    /// # Parameters
    /// 
    /// - `filepath` - The filepath to a Brainfuck source file.
    pub fn from_source(filepath: &str) -> Self {
        let lex: Result<Lexer, std::io::Error> = Lexer::from_source(filepath);
        if lex.is_err() {
            log_error!("failed to open file or read from file");
            std::process::exit(1);
        }

        Compiler {
            curr_tok: None,
            lexer: lex.unwrap(),
            curr_ln: 0_usize
        }
    }

    /// Compiles the Brainfuck source file from which this instance of the
    /// compiler was created from and returns a vector of all the BFVM assembly
    /// instructions.
    pub fn compile(&mut self) -> Vec<OpCode> {
        let mut code: Vec<OpCode> = Vec::<OpCode>::new();

        self.parse_program(&mut code);
        code
    }
    
    /// Parses the entire Brainfuck program and appends to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_program(&mut self, code: &mut Vec<OpCode>) {
        self.curr_tok = self.lexer.next_token();
        
        while self.curr_tok.is_some() {
            let token: Token = self.curr_tok.unwrap();
            match token {
                Token::Plus => self.parse_add_byte(code),
                Token::Minus => self.parse_sub_byte(code),
                Token::ArrowLeft => self.parse_sub_ptr(code),
                Token::ArrowRight => self.parse_add_ptr(code),
                Token::Dot => self.parse_write(code),
                Token::Comma => self.parse_read(code),
                Token::BracketLeft => self.parse_conditional(code),
                Token::BracketRight => {
                    log_error!("unexpected token: ']'");
                    std::process::exit(1);
                }
            }
        }

        code.push(OpCode::End);
    }

    /// Parses the Brainfuck commands for adding to a byte and appends the
    /// instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_add_byte(&mut self, code: &mut Vec<OpCode>) {
        let mut operand: u8 = 0_u8;

        while self.curr_tok.is_some() {
            let token: Token = self.curr_tok.unwrap();
            if token == Token::Plus {
                operand += 1;
            } else {
                break;
            }

            self.curr_tok = self.lexer.next_token();
        }

        code.push(OpCode::AddB(operand));
        self.curr_ln += 1;
    }

    /// Parses the Brainfuck commands for subtracting from a byte and appends
    /// the instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_sub_byte(&mut self, code: &mut Vec<OpCode>) {
        let mut operand: u8 = 0_u8;

        while self.curr_tok.is_some() {
            let token: Token = self.curr_tok.unwrap();
            if token == Token::Minus {
                operand += 1;
            } else {
                break;
            }

            self.curr_tok = self.lexer.next_token();
        }

        code.push(OpCode::SubB(operand));
        self.curr_ln += 1;
    }

    /// Parses the Brainfuck commands for adding to the data pointer and
    /// appends the instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_add_ptr(&mut self, code: &mut Vec<OpCode>) {
        let mut offset: usize = 0_usize;

        while self.curr_tok.is_some() {
            let token: Token = self.curr_tok.unwrap();
            if token == Token::ArrowRight {
                offset += 1;
            } else {
                break;
            }

            self.curr_tok = self.lexer.next_token();
        }

        code.push(OpCode::AddP(offset));
        self.curr_ln += 1;
    }

    /// Parses the Brainfuck commands for subtracting from the data pointer and
    /// appends the instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_sub_ptr(&mut self, code: &mut Vec<OpCode>) {
        let mut offset: usize = 0_usize;

        while self.curr_tok.is_some() {
            let token: Token = self.curr_tok.unwrap();
            if token == Token::ArrowLeft {
                offset += 1;
            } else {
                break;
            }

            self.curr_tok = self.lexer.next_token();
        }

        code.push(OpCode::SubP(offset));
        self.curr_ln += 1;
    }

    /// Parses the Brainfuck command to write a byte to `stdout` and appends
    /// the instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_write(&mut self, code: &mut Vec<OpCode>) {
        code.push(OpCode::Write);
        
        self.curr_tok = self.lexer.next_token();
        self.curr_ln += 1;
    }

    /// Parses the Brainfuck command to read a byte from `stdin` and appends
    /// the instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_read(&mut self, code: &mut Vec<OpCode>) {
        code.push(OpCode::Read);

        self.curr_tok = self.lexer.next_token();
        self.curr_ln += 1;
    }

    /// Pares the Brainfuck command for condtional statements and appends the
    /// instructions to `code`.
    /// 
    /// # Parameters
    /// 
    /// - `code` - The vector containing the assembly instructions.
    fn parse_conditional(&mut self, code: &mut Vec<OpCode>) {
        let mut braces: Vec<usize> = Vec::<usize>::new();

        braces.push(self.curr_ln);
        self.curr_ln += 1;

        code.push(OpCode::Jz(0_usize));

        self.curr_tok = self.lexer.next_token();
        while !braces.is_empty() {
            if self.curr_tok.is_none() {
                log_error!("bfvm: error: no matching ']'");
                std::process::exit(1);
            }

            let token: Token = self.curr_tok.unwrap();
            match token {
                Token::Plus => self.parse_add_byte(code),
                Token::Minus => self.parse_sub_byte(code),
                Token::ArrowLeft => self.parse_sub_ptr(code),
                Token::ArrowRight => self.parse_add_ptr(code),
                Token::Dot => self.parse_write(code),
                Token::Comma => self.parse_read(code),
                Token::BracketLeft => {
                    braces.push(self.curr_ln);
                    self.curr_ln += 1;

                    code.push(OpCode::Jz(0_usize));
                    self.curr_tok = self.lexer.next_token();
                },
                Token::BracketRight => {
                    let open: usize = braces
                        .pop()
                        .unwrap();

                    self.curr_ln += 1;
                    code[open] = OpCode::Jz(self.curr_ln);
                    code.push(OpCode::Jmp(open));

                    self.curr_tok = self.lexer.next_token();
                },
            }
        }
    }
}

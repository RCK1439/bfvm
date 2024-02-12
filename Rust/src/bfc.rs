use crate::lexer;

/// OpCodes to be used for the Brainf*ck Virtual Machine assembly language.
#[derive(PartialEq, Clone, Copy)]
pub enum OpCode {
    AddB,
    SubB,
    AddP,
    SubP,
    Write,
    Read,
    Jz,
    Jmp,
    End
}

/// Bytecode representation. This is a combination of an OpCode and an operand.
#[derive(Clone, Copy)]
pub struct ByteCode {
    pub code: OpCode,
    pub operand: usize
}

impl ByteCode {
    /// Creates a new instance of a ByteCode.
    /// 
    /// # Arguments
    /// 
    /// * `code` - The OpCode of the ByteCode.
    /// * `operand` - The operand of the ByteCode.
    /// 
    /// # Returns
    /// 
    /// An instance of a ByteCode object.
    pub fn new(code: OpCode, operand: usize) -> Self {
        ByteCode {
            code,
            operand
        }
    }
}

pub struct Compiler {
    current_line: usize,
    lexer: lexer::LexicalAnalyzer,
    token: lexer::Token
}

impl Compiler {
    /// Creates a new instance of the BFVM compiler.
    /// 
    /// # Arguments
    /// 
    /// `filepath` - The path to a Brainf*ck source file.
    /// 
    /// # Returns
    /// 
    /// An instance of the BFC (Brainf*ck compiler).
    pub fn new(filepath: &str) -> Self {
        Compiler {
            current_line: 0,
            lexer: lexer::LexicalAnalyzer::new(filepath),
            token: lexer::Token::None
        }
    }

    /// Compiles the code within the Brainf*ck source file used to create
    /// this instance of the BFC to the BFVM assembly language.
    /// 
    /// # Returns
    /// 
    /// A vector of the BFVM assembly instructions in the order to be executed
    /// in.
    pub fn compile(&mut self) -> Vec<ByteCode> {
        let mut byte_code: Vec<ByteCode> = Vec::<ByteCode>::new();

        self.parse_program(&mut byte_code);
        byte_code
    }

    /// Parses the whole Brainf*ck source file.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_program(&mut self, byte_code: &mut Vec<ByteCode>) {
        self.token = self.lexer.get_token();

        while self.token != lexer::Token::EndOfFile {
            match self.token {
                lexer::Token::Plus => self.parse_add_byte(byte_code),
                lexer::Token::Minus => self.parse_sub_byte(byte_code),
                lexer::Token::ArrowLeft => self.parse_sub_ptr(byte_code),
                lexer::Token::ArrowRight => self.parse_add_ptr(byte_code),
                lexer::Token::Dot => self.parse_write(byte_code),
                lexer::Token::Comma => self.parse_read(byte_code),
                lexer::Token::BracketLeft => self.parse_conditional(byte_code),
                _ => debug_assert!(false)
            }
        }

        byte_code.push(ByteCode::new(OpCode::End, 0));
    }

    /// Parses contiguous `+` instructions of the Brainf*ck programming
    /// language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_add_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::Plus {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::AddB, op));
        self.current_line += 1;
    }

    /// Parses contiguous `-` instructions of the Brainf*ck programming
    /// language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_sub_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::Minus {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::SubB, op));
        self.current_line += 1;
    }

    /// Parses contiguous `<` instructions of the Brainf*ck programming
    /// language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_sub_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::ArrowLeft {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::SubP, op));
        self.current_line += 1;
    }

    /// Parses contiguous `>` instructions of the Brainf*ck programming
    /// language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_add_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::ArrowRight {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::AddP, op));

        self.current_line += 1;
    }

    /// Parses the `.` instruction of the Brainf*ck programming language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_write(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode::new(OpCode::Write, 0));

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

    /// Parses the `,` instruction of the Brainf*ck programming language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_read(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode::new(OpCode::Read, 0));

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

    /// Parses the control flow (`[`/`]`) operations in the Brainf*ck
    /// programming language.
    /// 
    /// # Arguments
    /// 
    /// `byte_code` - The vector to append the BFVM assembly instructions to.
    fn parse_conditional(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut braces: Vec<usize> = Vec::<usize>::new();

        braces.push(self.current_line);
        byte_code.push(ByteCode::new(OpCode::Jz, 0));

        self.current_line += 1;
        self.token = self.lexer.get_token();
        while !braces.is_empty() {
            if self.token == lexer::Token::EndOfFile {
                eprintln!("bfc: error: unbalanced braces");
                std::process::exit(1);
            }

            match self.token {
                lexer::Token::Plus => self.parse_add_byte(byte_code),
                lexer::Token::Minus => self.parse_sub_byte(byte_code),
                lexer::Token::ArrowLeft => self.parse_sub_ptr(byte_code),
                lexer::Token::ArrowRight => self.parse_add_ptr(byte_code),
                lexer::Token::Dot => self.parse_write(byte_code),
                lexer::Token::Comma => self.parse_read(byte_code),
                lexer::Token::BracketLeft => {
                    braces.push(self.current_line);
                    byte_code.push(ByteCode::new(OpCode::Jz, 0));

                    self.current_line += 1;
                    self.token = self.lexer.get_token();    
                },
                lexer::Token::BracketRight => {
                    let open: usize = braces.pop()
                        .unwrap();

                    byte_code[open].operand = self.current_line + 1;
                    byte_code.push(ByteCode::new(OpCode::Jmp, open));

                    self.current_line += 1;
                    self.token = self.lexer.get_token();
                },
                _ => debug_assert!(false)
            }
        }
    }
}
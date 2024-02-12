use crate::lexer;

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

#[derive(Clone, Copy)]
pub struct ByteCode {
    pub code: OpCode,
    pub operand: usize
}

impl ByteCode {
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
    pub fn new(filepath: &str) -> Compiler {
        Compiler {
            current_line: 0,
            lexer: lexer::LexicalAnalyzer::init(filepath),
            token: lexer::Token::None
        }
    }

    pub fn compile(&mut self) -> Vec<ByteCode> {
        let mut byte_code: Vec<ByteCode> = Vec::<ByteCode>::new();

        self.parse_program(&mut byte_code);
        byte_code
    }

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

    fn parse_add_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::Plus {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::AddB, op));
        self.current_line += 1;
    }

    fn parse_sub_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::Minus {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::SubB, op));
        self.current_line += 1;
    }

    fn parse_sub_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::ArrowLeft {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::SubP, op));
        self.current_line += 1;
    }

    fn parse_add_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut op: usize = 0;

        while self.token == lexer::Token::ArrowRight {
            op += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(ByteCode::new(OpCode::AddP, op));

        self.current_line += 1;
    }

    fn parse_write(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode::new(OpCode::Write, 0));

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

    fn parse_read(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode::new(OpCode::Read, 0));

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

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
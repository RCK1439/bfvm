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

pub struct Compiler {
    current_line: usize,
    lexer: lexer::LexicalAnalyzer,
    token: lexer::Token
}

impl Compiler {
    pub fn new(filepath: &str) -> Compiler {
        return Compiler {
            current_line: 0,
            lexer: lexer::LexicalAnalyzer::init(filepath),
            token: lexer::Token::None
        };
    }

    pub fn compile(&mut self) -> Vec<ByteCode> {
        let mut byte_code: Vec<ByteCode> = Vec::<ByteCode>::new();

        self.parse_program(&mut byte_code);
        return byte_code;
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

        byte_code.push(ByteCode {
            code: OpCode::End,
            operand: 0
        });
    }

    fn parse_add_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut code: ByteCode = ByteCode {
            code: OpCode::AddB,
            operand: 0
        };

        while self.token == lexer::Token::Plus {
            code.operand += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(code);
        self.current_line += 1;
    }

    fn parse_sub_byte(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut code: ByteCode = ByteCode {
            code: OpCode::SubB,
            operand: 0
        };

        while self.token == lexer::Token::Minus {
            code.operand += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(code);
        self.current_line += 1;
    }

    fn parse_sub_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut code: ByteCode = ByteCode {
            code: OpCode::SubP,
            operand: 0
        };

        while self.token == lexer::Token::ArrowLeft {
            code.operand += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(code);
        self.current_line += 1;
    }

    fn parse_add_ptr(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut code: ByteCode = ByteCode {
            code: OpCode::AddP,
            operand: 0
        };

        while self.token == lexer::Token::ArrowRight {
            code.operand += 1;
            self.token = self.lexer.get_token();
        }

        byte_code.push(code);

        self.current_line += 1;
    }

    fn parse_write(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode {
            code: OpCode::Write,
            operand: 0
        });

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

    fn parse_read(&mut self, byte_code: &mut Vec<ByteCode>) {
        byte_code.push(ByteCode {
            code: OpCode::Read,
            operand: 0
        });

        self.current_line += 1;
        self.token = self.lexer.get_token();
    }

    fn parse_conditional(&mut self, byte_code: &mut Vec<ByteCode>) {
        let mut braces: Vec<usize> = Vec::<usize>::new();

        braces.push(self.current_line);
        byte_code.push(ByteCode {
            code: OpCode::Jz,
            operand: 0
        });

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
                    byte_code.push(ByteCode {
                        code: OpCode::Jz,
                        operand: 0
                    });

                    self.current_line += 1;
                    self.token = self.lexer.get_token();    
                },
                lexer::Token::BracketRight => {
                    let open: usize = braces.pop()
                        .unwrap();

                    byte_code[open].operand = self.current_line + 1;
                    byte_code.push(ByteCode {
                        code: OpCode::Jmp,
                        operand: open
                    });

                    self.current_line += 1;
                    self.token = self.lexer.get_token();
                },
                _ => debug_assert!(false)
            }
        }
    }
}
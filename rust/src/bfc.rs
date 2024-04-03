use crate::{lexer::{Lexer, Token}, log_error, log_fatal};

#[derive(PartialEq, Clone, Copy, Debug)]
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

pub struct Compiler {
    token: Token,
    lexer: Lexer,
    currln: usize
}

impl Compiler {
    pub fn from_source(filepath: &str) -> Self {
        if let Ok(lexer) = Lexer::from_source(filepath) {
            Self {
                token: Token::None,
                lexer,
                currln: 0usize
            }
        } else {
            log_fatal!("failed to open file or read from file: {filepath}");
            std::process::exit(1);
        }
    }

    pub fn compile(&mut self) -> Vec<OpCode> {
        let mut code: Vec<OpCode> = Vec::<OpCode>::new();

        self.token = self.lexer.next_token();
        self.parse_program(&mut code);

        code
    }

    fn parse_program(&mut self, code: &mut Vec<OpCode>) {
        while self.token != Token::EndOfFile {
            match self.token {
                Token::Plus => self.parse_add_byte(code),
                Token::Minus => self.parse_sub_byte(code),
                Token::ArrowLeft => self.parse_sub_ptr(code),
                Token::ArrowRight => self.parse_add_ptr(code),
                Token::Dot => self.parse_write(code),
                Token::Comma => self.parse_read(code),
                Token::BracketLeft => self.parse_conditional(code),
                _ => {
                    log_error!("unexpected token: {:?}", self.token);
                    std::process::exit(1);
                }
            }
        }

        code.push(OpCode::End);
    }

    fn parse_add_byte(&mut self, code: &mut Vec<OpCode>) {
        let mut operand: u8 = 0u8;

        while self.token == Token::Plus {
            operand += 1;
            self.token = self.lexer.next_token();
        }

        code.push(OpCode::AddB(operand));
        self.currln += 1;
    }

    fn parse_sub_byte(&mut self, code: &mut Vec<OpCode>) {
        let mut operand: u8 = 0u8;

        while self.token == Token::Minus {
            operand += 1;
            self.token = self.lexer.next_token();
        }

        code.push(OpCode::SubB(operand));
        self.currln += 1;
    }

    fn parse_add_ptr(&mut self, code: &mut Vec<OpCode>) {
        let mut offset: usize = 0usize;

        while self.token == Token::ArrowRight {
            offset += 1;
            self.token = self.lexer.next_token();
        }

        code.push(OpCode::AddP(offset));
        self.currln += 1;
    }

    fn parse_sub_ptr(&mut self, code: &mut Vec<OpCode>) {
        let mut offset: usize = 0usize;

        while self.token == Token::ArrowLeft {
            offset += 1;
            self.token = self.lexer.next_token();
        }

        code.push(OpCode::SubP(offset));
        self.currln += 1;
    }

    fn parse_write(&mut self, code: &mut Vec<OpCode>) {
        code.push(OpCode::Write);
        
        self.token = self.lexer.next_token();
        self.currln += 1;
    }

    fn parse_read(&mut self, code: &mut Vec<OpCode>) {
        code.push(OpCode::Read);

        self.token = self.lexer.next_token();
        self.currln += 1;
    }

    fn parse_conditional(&mut self, code: &mut Vec<OpCode>) {
        let mut braces: Vec<usize> = Vec::<usize>::new();

        braces.push(self.currln);
        self.currln += 1;

        code.push(OpCode::Jz(0usize));

        self.token = self.lexer.next_token();
        while !braces.is_empty() {
            if self.token == Token::EndOfFile {
                log_error!("no matching ']'");
                std::process::exit(1);
            }

            match self.token {
                Token::Plus => self.parse_add_byte(code),
                Token::Minus => self.parse_sub_byte(code),
                Token::ArrowLeft => self.parse_sub_ptr(code),
                Token::ArrowRight => self.parse_add_ptr(code),
                Token::Dot => self.parse_write(code),
                Token::Comma => self.parse_read(code),
                Token::BracketLeft => {
                    braces.push(self.currln);
                    self.currln += 1;

                    code.push(OpCode::Jz(0usize));
                    self.token = self.lexer.next_token();
                },
                Token::BracketRight => {
                    if let Some(open) = braces.pop() {
                        self.currln += 1;
                        code[open] = OpCode::Jz(self.currln);
                        code.push(OpCode::Jmp(open));

                        self.token = self.lexer.next_token();
                    } else {
                        log_fatal!("stack empty");
                        std::process::exit(1);
                    }
                },
                _ => {
                    log_error!("unexpected token: {:?}", self.token);
                    std::process::exit(1);
                }
            }
        }
    }
}

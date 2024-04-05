use crate::lexer::{Lexer, Token};

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
    pub fn from_source(filepath: &str) -> Result<Self, std::io::Error> {
        Ok(Self {
            token: Token::None,
            lexer: Lexer::from_source(filepath)?,
            currln: 0usize
        })
    }

    pub fn compile(&mut self) -> Result<Vec<OpCode>, String> {
        let mut code: Vec<OpCode> = Vec::<OpCode>::new();

        self.token = self.lexer.next_token();
        self.parse_program(&mut code)?;

        Ok(code)
    }

    fn parse_program(&mut self, code: &mut Vec<OpCode>) -> Result<(), String> {
        while self.token != Token::EndOfFile {
            match self.token {
                Token::Plus => self.parse_add_byte(code),
                Token::Minus => self.parse_sub_byte(code),
                Token::ArrowLeft => self.parse_sub_ptr(code),
                Token::ArrowRight => self.parse_add_ptr(code),
                Token::Dot => self.parse_write(code),
                Token::Comma => self.parse_read(code),
                Token::BracketLeft => self.parse_conditional(code)?,
                _ => return Err(format!("unexpected token: {:?}", self.token))
            }
        }

        code.push(OpCode::End);
        Ok(())
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

    fn parse_conditional(&mut self, code: &mut Vec<OpCode>) -> Result<(), String> {
        let mut braces: Vec<usize> = Vec::<usize>::new();

        braces.push(self.currln);
        self.currln += 1;

        code.push(OpCode::Jz(0usize));

        self.token = self.lexer.next_token();
        while !braces.is_empty() {
            if self.token == Token::EndOfFile {
                return Err(String::from("no matching ']'"));
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
                        return Err(String::from("stack empty"));
                    }
                },
                _ => return Err(format!("unexpected token: {:?}", self.token))
            }
        }

        Ok(())
    }
}

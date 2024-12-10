///
/// Author: Ruan C. Keet
/// Date: 2024-09-22
/// 

pub mod lexer;

use crate::prelude::*;
use crate::error::BFVMError;

use lexer::Lexer;
use lexer::token::Token;

// --- opcode definition ------------------------------------------------------

/// This enum represents the instructions generated by the JIT compiler
#[derive(PartialEq, Clone, Copy, Debug)]
pub enum OpCode {
    AddB(u8),
    SubB(u8),
    AddP(u16),
    SubP(u16),
    Write,
    Read,
    Jmp(usize),
    Jz(usize),
    End
}

// --- compiler definition ----------------------------------------------------

/// This struct represents the JIT compiler that compiles the Brainfuck source
/// code
pub struct Compiler {
    /// The current token
    token: Token,
    /// The lexical analyzer getting getting the tokens
    lexer: Lexer,
    /// The current line in the generated instructions so far
    currln: usize
}

impl Compiler {
    /// Creates an instance of `Compiler` from a filepath to a Brainfuck source
    /// file
    /// 
    /// # Params
    /// 
    /// - `filepath` - A path to a Brainfuck source file
    /// 
    /// # Errors
    /// 
    /// - If the source file does not exist
    /// - If the contents of the source file could not be read
    pub fn new(filepath: &str) -> Result<Self> {
        Ok(Self {
            token: Token::None,
            lexer: Lexer::new(filepath)?,
            currln: 0usize
        })
    }

    /// JIT compiles the Brainfuck source contents
    /// 
    /// # Errors
    /// 
    /// If there were any syntax errors in the source code
    pub fn compile(&mut self) -> Result<Vec<OpCode>> {
        let mut code = Vec::new();

        self.token = self.lexer.next_token()?;
        self.parse_program(&mut code)?;

        Ok(code)
    }

    /// Parses the whole Brainfuck program
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_program(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        while self.token != Token::EndOfFile {
            match self.token {
                Token::Plus => self.parse_add_byte(code)?,
                Token::Minus => self.parse_sub_byte(code)?,
                Token::ArrowLeft => self.parse_sub_ptr(code)?,
                Token::ArrowRight => self.parse_add_ptr(code)?,
                Token::Dot => self.parse_write(code)?,
                Token::Comma => self.parse_read(code)?,
                Token::BracketLeft => self.parse_conditional(code)?,
                _ => {
                    let pos = self.lexer.position();
                    return Err(BFVMError::UnexpectedToken(pos, self.token.clone()));
                }
            }
        }

        code.push(OpCode::End);
        Ok(())
    }

    /// Parses a string of `+` commands
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_add_byte(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        let mut operand = 0u8;

        while self.token == Token::Plus {
            operand += 1;
            self.token = self.lexer.next_token()?;
        }

        code.push(OpCode::AddB(operand));
        self.currln += 1;

        Ok(())
    }

    /// Parses a string of `-` commands
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_sub_byte(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        let mut operand = 0u8;

        while self.token == Token::Minus {
            operand += 1;
            self.token = self.lexer.next_token()?;
        }

        code.push(OpCode::SubB(operand));
        self.currln += 1;

        Ok(())
    }

    /// Parses a string of `>` commands
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_add_ptr(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        let mut offset = 0;

        while self.token == Token::ArrowRight {
            offset += 1;
            self.token = self.lexer.next_token()?;
        }

        code.push(OpCode::AddP(offset));
        self.currln += 1;

        Ok(())
    }

    /// Parses a string of `<` commands
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_sub_ptr(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        let mut offset = 0;

        while self.token == Token::ArrowLeft {
            offset += 1;
            self.token = self.lexer.next_token()?;
        }

        code.push(OpCode::SubP(offset));
        self.currln += 1;

        Ok(())
    }

    /// Parses the write command
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_write(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        code.push(OpCode::Write);
        
        self.token = self.lexer.next_token()?;
        self.currln += 1;

        Ok(())
    }

    /// Parses the read command
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// If an unknown operator was found
    fn parse_read(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        code.push(OpCode::Read);

        self.token = self.lexer.next_token()?;
        self.currln += 1;

        Ok(())
    }

    /// Parses any control-flow statements
    /// 
    /// # Params
    /// 
    /// - `code` - The generated output code
    /// 
    /// # Errors
    /// 
    /// - If there were any syntax errors in the source code
    /// - If an unexpected token was found
    /// - If the loop was not closed
    /// - If the stack is empty whilst parsing
    fn parse_conditional(&mut self, code: &mut Vec<OpCode>) -> Result<()> {
        let mut braces = Vec::new();

        braces.push((self.currln, self.lexer.position()));
        self.currln += 1;

        code.push(OpCode::Jz(0));

        self.token = self.lexer.next_token()?;
        while !braces.is_empty() {
            match self.token {
                Token::Plus => self.parse_add_byte(code)?,
                Token::Minus => self.parse_sub_byte(code)?,
                Token::ArrowLeft => self.parse_sub_ptr(code)?,
                Token::ArrowRight => self.parse_add_ptr(code)?,
                Token::Dot => self.parse_write(code)?,
                Token::Comma => self.parse_read(code)?,
                Token::BracketLeft => {
                    braces.push((self.currln, self.lexer.position()));
                    self.currln += 1;

                    code.push(OpCode::Jz(0));
                    self.token = self.lexer.next_token()?;
                },
                Token::BracketRight => {
                    if let Some((open, _)) = braces.pop() {
                        self.currln += 1;
                        code[open] = OpCode::Jz(self.currln);
                        code.push(OpCode::Jmp(open));

                        self.token = self.lexer.next_token()?;
                    } else {
                        let pos = self.lexer.position();
                        return Err(BFVMError::UnexpectedToken(pos, self.token.clone()));
                    }
                },
                Token::EndOfFile => {
                    if let Some((_, pos)) = braces.pop() {
                        return Err(BFVMError::LoopNotClosed(pos));
                    } else {
                        let pos = self.lexer.position();
                        return Err(BFVMError::StackEmpty(pos));
                    }
                },
                // This suggests that Token::None was found, which should never happen
                _ => {
                    let pos = self.lexer.position();
                    return Err(BFVMError::UnexpectedToken(pos, self.token.clone()));
                }
            }
        }

        Ok(())
    }
}

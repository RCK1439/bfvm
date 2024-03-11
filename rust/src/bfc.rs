use crate::lexer::{Lexer, Token};

#[derive(PartialEq)]
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

pub fn compile(filepath: &str) -> Vec<OpCode> {
    let lex: Result<Lexer, &str> = Lexer::from_source(filepath);
    if lex.is_err() {
        let lex_err: &str = lex
            .err()
            .unwrap();
        println!("bfvm: error: {lex_err}");
        std::process::exit(0);
    }

    let mut lexer: Lexer = lex.unwrap();
    let mut code: Vec<OpCode> = Vec::<OpCode>::new();
    
    parse_program(&mut lexer, &mut code);
    code.push(OpCode::End);

    code
}

fn parse_program(lexer: &mut  Lexer, code: &mut Vec<OpCode>) {
    let mut tok: Option<Token> = lexer.next_token();

    while tok.is_some() {
        let token: Token = tok.unwrap();
        match token {
            Token::Plus => parse_add_byte(lexer, code),
            Token::Minus => parse_sub_byte(lexer, code),
            Token::ArrowLeft => parse_add_ptr(lexer, code),
            Token::ArrowRight => parse_sub_ptr(lexer, code),
            Token::Dot => parse_write(code),
            Token::Comma => parse_read(code),
            Token::BracketLeft => parse_conditional(lexer, code),
            _ => {
                println!("bfvm: error: invalid token");
                std::process::exit(0);
            }
        }

        tok = lexer.next_token();
    }
}

fn parse_add_byte(lexer: &mut Lexer, code: &mut Vec<OpCode>) {
    let mut operand: u8 = 1_u8;
    let mut tok: Option<Token> = lexer.next_token();

    while tok.is_some() {
        let token: Token = tok.unwrap();

        if token == Token::Plus {
            operand += 1;
        } else {
            break;
        }

        tok = lexer.next_token();
    }

    code.push(OpCode::AddB(operand));
}

fn parse_sub_byte(lexer: &mut Lexer, code: &mut Vec<OpCode>) {
    let mut operand: u8 = 1_u8;
    let mut tok: Option<Token> = lexer.next_token();

    while tok.is_some() {
        let token: Token = tok.unwrap();

        if token == Token::Minus {
            operand += 1;
        } else {
            break;
        }

        tok = lexer.next_token();
    }

    code.push(OpCode::SubB(operand));
}

fn parse_add_ptr(lexer: &mut Lexer, code: &mut Vec<OpCode>) {
    let mut operand: usize = 1_usize;
    let mut tok: Option<Token> = lexer.next_token();

    while tok.is_some() {
        let token: Token = tok.unwrap();

        if token == Token::ArrowRight {
            operand += 1;
        } else {
            break;
        }

        tok = lexer.next_token();
    }

    code.push(OpCode::AddP(operand));
}

fn parse_sub_ptr(lexer: &mut Lexer, code: &mut Vec<OpCode>) {
    let mut operand: usize = 1_usize;
    let mut tok: Option<Token> = lexer.next_token();

    while tok.is_some() {
        let token: Token = tok.unwrap();

        if token == Token::ArrowLeft {
            operand += 1;
        } else {
            break;
        }

        tok = lexer.next_token();
    }

    code.push(OpCode::SubP(operand));
}

fn parse_write(code: &mut Vec<OpCode>) {
    code.push(OpCode::Write);
}

fn parse_read(code: &mut Vec<OpCode>) {
    code.push(OpCode::Read);
}

fn parse_conditional(lexer: &mut Lexer, code: &mut Vec<OpCode>) {
    let mut braces: Vec<usize> = Vec::<usize>::new();

    braces.push(code.len() - 1);
    code.push(OpCode::Jz(0_usize));

    while !braces.is_empty() {
        let tok: Option<Token> = lexer.next_token();
        if tok.is_none() {
            println!("bfvm: error: no matching ']'");
            std::process::exit(0);
        }
        
        let token: Token = tok.unwrap();
        match token {
            Token::Plus => parse_add_byte(lexer, code),
            Token::Minus => parse_sub_byte(lexer, code),
            Token::ArrowLeft => parse_add_ptr(lexer, code),
            Token::ArrowRight => parse_sub_ptr(lexer, code),
            Token::Dot => parse_write(code),
            Token::Comma => parse_read(code),
            Token::BracketLeft => {
                code.push(OpCode::Jz(0_usize));
            },
            Token::BracketRight => {
                let top: Option<usize> = braces.pop();
                if top.is_none() {
                    println!("bfvm: error: bracket stack emptry");
                    std::process::exit(0);
                }

                let open: usize = top.unwrap();
                code[open] = OpCode::Jz(code.len() - 1);
                code.push(OpCode::Jmp(open));
            }
        }
    } 
}
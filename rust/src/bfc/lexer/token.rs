use crate::prelude::*;
use crate::error::BFVMError;

// --- token definition ---------------------------------------------------------------------------

/// This enum represents the different kind of valid tokens to be found in a
/// Brainfuck source file.
#[derive(PartialEq, Debug, Clone)]
pub enum Token {
    None,
    Plus,
    Minus,
    ArrowLeft,
    ArrowRight,
    Dot,
    Comma,
    BracketLeft,
    BracketRight,
    EndOfFile
}

impl Token {
    /// Gets the lexeme represented by the token.
    pub fn lexeme(&self) -> &str {
        match self {
            Token::None => "none",
            Token::Plus => "'+'",
            Token::Minus => "'-'",
            Token::ArrowLeft => "'<'",
            Token::ArrowRight => "'>'",
            Token::Dot => "'.'",
            Token::Comma => "','",
            Token::BracketLeft => "'['",
            Token::BracketRight => "']'",
            Token::EndOfFile => "end-of-file",
        }
    }
}

// --- traits -------------------------------------------------------------------------------------

pub trait BrainfuckOperator {
    /// Checks if `self` is a valid Brainfuck command.
    fn is_bf_operator(&self) -> bool;

    /// Creates a Brainfuck token from the value of `self`
    /// 
    /// # Errors
    /// If `self` is not a valid Brainfuck operator
    fn bf_token(&self) -> Result<Token>;
}

impl BrainfuckOperator for char {
    fn is_bf_operator(&self) -> bool {
        *self == '+' || *self == '-' || *self == '<' || *self == '>' ||
        *self == '.' || *self == ',' || *self == '[' || *self == ']'
    }
    
    fn bf_token(&self) -> Result<Token> {
        match self {
            '+' => Ok(Token::Plus),
            '-' => Ok(Token::Minus),
            '<' => Ok(Token::ArrowLeft),
            '>' => Ok(Token::ArrowRight),
            '.' => Ok(Token::Dot),
            ',' => Ok(Token::Comma),
            '[' => Ok(Token::BracketLeft),
            ']' => Ok(Token::BracketRight),
            _ => Err(BFVMError::NotABrainfuckOperator(*self))
        }
    }
}

use crate::prelude::*;
use crate::error::BFVMError;

// --- token definition -------------------------------------------------------

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

impl TryFrom<char> for Token {
    type Error = BFVMError;

    /// Attempts to create a token from a `char`
    /// 
    /// Valid char's are: `+`, `-`, `<`, `>`, `.`, `,`, `[`, `]`
    /// 
    /// # Errors
    /// 
    /// If a valid char was not given as an argument
    fn try_from(ch: char) -> Result<Self> {
        match ch {
            '+' => Ok(Token::Plus),
            '-' => Ok(Token::Minus),
            '<' => Ok(Token::ArrowLeft),
            '>' => Ok(Token::ArrowRight),
            '.' => Ok(Token::Dot),
            ',' => Ok(Token::Comma),
            '[' => Ok(Token::BracketLeft),
            ']' => Ok(Token::BracketRight),
            _ => Err(BFVMError::NotABrainfuckOperator(ch))
        }
    }
}

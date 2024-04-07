use crate::error::{BFVMError, BFVMErrSeverity};

use std::fs;

/// This enum represents the different kind of valid tokens to be found in a
/// Brainfuck source file.
#[derive(PartialEq, Debug)]
pub enum Token {
    None         = 0x00,
    Plus         = 0x2B,
    Minus        = 0x2D,
    ArrowLeft    = 0x3C,
    ArrowRight   = 0x3E,
    Dot          = 0x2E,
    Comma        = 0x2C,
    BracketLeft  = 0x5B,
    BracketRight = 0x5D,
    EndOfFile    = 0xFF
}

/// This struct represents the lexical analyzer for the JIT compiler used in
/// BFVM.
pub struct Lexer {
    /// This is the content read from the Brainfuck source file.
    source: String,

    /// The index of the current character in `source`.
    curr: usize
}

impl Lexer {
    /// Creates an instance of `Lexer` constructed from a filepath.
    /// 
    /// # Parameters
    /// 
    /// - `filepath` - The path to a Brainfuck source file.
    /// 
    /// # Errors
    /// 
    /// - If the source file does not exist.
    /// - If the contents of the file could not be read.
    pub fn from_source(filepath: &str) -> Result<Self, BFVMError> {
        if let Ok(source) = fs::read_to_string(filepath) {
            Ok(Self {
                source,
                curr: 0usize
            })
        } else {
            Err(BFVMError::new(format!("failed reading from file: {filepath}"), BFVMErrSeverity::Error))
        }
    }

    /// Retrieves the next token from the content of the source file.
    pub fn next_token(&mut self) -> Token {
        loop {
            if let Some(ch) = self.next_character() {
                if is_bf_command(ch) {
                    return match ch {
                        '+' => Token::Plus,
                        '-' => Token::Minus,
                        '<' => Token::ArrowLeft,
                        '>' => Token::ArrowRight,
                        '.' => Token::Dot,
                        ',' => Token::Comma,
                        '[' => Token::BracketLeft,
                        ']' => Token::BracketRight,
                        _ => panic!("unknown symbol: '{ch}'")
                    };
                }
            } else {
                return Token::EndOfFile;
            }
        }
    }

    /// Retrieves the next character from the content of the source file.
    /// This will return `None` if the end of the file has been reached and
    /// `Some` otherwise.
    fn next_character(&mut self) -> Option<char> {
        let ch: char = self.source
            .chars()
            .nth(self.curr)?;

        self.curr += 1;
        Some(ch)
    }
}

/// Checks if `ch` is a valid Brainfuck command.
/// 
/// # Parameters
/// 
/// - `ch` - The character to check for.
#[inline(always)]
fn is_bf_command(ch: char) -> bool {
    ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
    ch == '.' || ch == ',' || ch == '[' || ch == ']'
}

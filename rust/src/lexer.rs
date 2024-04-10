use crate::error::BFVMError;

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

/// This struct represents the lexer's current position in the source code.
#[derive(Clone, Copy, Debug)]
pub struct SourcePosition {
    /// The current line position in the source code.
    pub line: usize,

    /// The current column position in the source code.
    pub column: usize
}

impl SourcePosition {

    /// Creates a new instance of the `SourcePosition` from the very start
    /// or beginning of the source file.
    #[inline(always)]
    pub fn begin() -> Self {
        Self {
            line: 1,
            column: 0
        }
    }

    /// Proceeds to the next column of the source file.
    #[inline(always)]
    pub fn next_column(&mut self) {
        self.column += 1;
    }

    /// Proceeds to the next line of the source file.
    #[inline(always)]
    pub fn next_line(&mut self) {
        self.line += 1;
        self.column = 1;
    }
}

/// This struct represents the lexical analyzer for the JIT compiler used in
/// BFVM.
pub struct Lexer {
    /// This is the content read from the Brainfuck source file.
    source: String,

    /// The index of the current character in `source`.
    curr: usize,

    /// The current position of the lexer in `source`.
    position: SourcePosition
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
        let source: String = fs::read_to_string(filepath)
            .map_err(|_| BFVMError::Error(format!("failed reading from file: {filepath}")))?;
        
        Ok(Self {
            source,
            curr: 0usize,
            position: SourcePosition::begin()
        })
    }

    /// Retrieves the next token from the content of the source file.
    /// 
    /// # Panics
    /// 
    /// If the current character somehow makes it to the catch-all end of the
    /// match statement.
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

    /// Gets the current position of the lexer in the source content.
    #[inline(always)]
    pub fn current_position(&self) -> SourcePosition {
        self.position
    }

    /// Retrieves the next character from the content of the source file.
    /// This will return `None` if the end of the file has been reached and
    /// `Some` otherwise.
    fn next_character(&mut self) -> Option<char> {
        let ch: char = self.source
            .chars()
            .nth(self.curr)?;

        self.curr += 1;
        if ch == '\n' {
            self.position.next_line();
        } else {
            self.position.next_column();
        }
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

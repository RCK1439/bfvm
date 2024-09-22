pub mod token;

use token::{ Token, BrainfuckOperator };

use crate::prelude::*;
use crate::error::BFVMError;

// --- source position definition -----------------------------------------------------------------

/// This struct represents the lexer's current position in the source code.
#[derive(Clone, Copy, Debug)]
pub struct SourcePosition {
    /// The current line position in the source code.
    pub line: usize,

    /// The current column position in the source code.
    pub column: usize
}

impl SourcePosition {
    /// Creates a new instance of the `SourcePosition` from the very start or beginning of the
    /// source file.
    pub fn new() -> Self {
        Self {
            line: 1,
            column: 0
        }
    }

    /// Proceeds to the next column of the source file.
    pub fn next_column(&mut self) {
        self.column += 1;
    }

    /// Proceeds to the next line of the source file.
    pub fn next_line(&mut self) {
        self.line += 1;
        self.column = 1;
    }
}

// --- lexical analyzer definition ----------------------------------------------------------------

/// This struct represents the lexical analyzer for the JIT compiler used in BFVM.
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
    /// # Params
    /// - `filepath` - The path to a Brainfuck source file.
    /// 
    /// # Errors
    /// - If the source file does not exist.
    /// - If the contents of the file could not be read.
    pub fn new(filepath: &str) -> Result<Self> {
        let source = std::fs::read_to_string(filepath)
            .map_err(|_| BFVMError::FailedReadingFromFile(filepath.to_string()))?;
        
        Ok(Self {
            source,
            curr: 0usize,
            position: SourcePosition::new()
        })
    }

    /// Retrieves the next token from the content of the source file.
    /// 
    /// # Panics
    /// If the current character somehow makes it to the catch-all end of the
    /// match statement.
    pub fn next_token(&mut self) -> Result<Token> {
        while let Some(ch) = self.next_character() {
            if !ch.is_bf_operator() {
                continue;
            }

            return ch.bf_token();
        }

        Ok(Token::EndOfFile)
    }

    /// Gets the current position of the lexer in the source content.
    pub fn position(&self) -> SourcePosition {
        self.position
    }

    /// Retrieves the next character from the content of the source file. This will return [None]
    /// if the end of the file has been reached and [Some] otherwise.
    fn next_character(&mut self) -> Option<char> {
        if let Some(ch) = self.source.chars().nth(self.curr) {
            self.curr += 1;
            if ch == '\n' {
                self.position.next_line();
            } else {
                self.position.next_column();
            }

            return Some(ch);
        }

        None
    }
}

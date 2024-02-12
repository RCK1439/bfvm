use std::io::{BufReader, Read};
use std::fs::File;

/// Representation of the tokens to be found in a Brainf*ck sourve file.
#[derive(PartialEq)]
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

/// Representation of the Lexer used by the Brainf*ck compiler.
pub struct LexicalAnalyzer {
    content: String,
    curr: usize
}

impl LexicalAnalyzer {
    /// Creates a new instance of the LexicalAnalyzer.
    /// 
    /// # Arguments
    /// 
    /// `filepath` - The path to a Brainf*ck source file.
    /// 
    /// # Returns
    /// 
    /// A new instance of the LexicalAnalyzer.
    pub fn new(filepath: &str) -> Self {
        let file: File = std::fs::File::open(filepath)
            .expect("failed to open file");

        let mut reader: BufReader<File> = std::io::BufReader::new(file);
        let mut data: String = String::new();

        reader.read_to_string(&mut data)
            .expect("failed to read data");

        LexicalAnalyzer {
            content: data,
            curr: 0
        }
    }

    /// Retrieves the next token from the Brainf*ck source file.
    /// 
    /// # Returns
    /// 
    /// The next token from the source file.
    pub fn get_token(&mut self) -> Token {
        loop {
            if self.curr >= self.content.len() {
                return Token::EndOfFile;
            }

            let ch: char = self.content
                .chars()
                .nth(self.curr)
                .unwrap();

            self.curr += 1;
            if self.is_instruction(ch) {
                return match ch {
                    '+' => Token::Plus,
                    '-' => Token::Minus,
                    '<' => Token::ArrowLeft,
                    '>' => Token::ArrowRight,
                    '.' => Token::Dot,
                    ',' => Token::Comma,
                    '[' => Token::BracketLeft,
                    ']' => Token::BracketRight,
                    _ => Token::None // This should never happen!
                };
            }
        }
    }

    /// Checks if `ch` is a valid instruction in the Brainf*ck programming
    /// language.
    /// 
    /// # Arguments
    /// 
    /// `ch` - The character to check for.
    /// 
    /// # Returns
    /// 
    /// `true`, if `ch` is a valid Brainf*ck instruction. `false`, otherwise.
    #[inline(always)]
    fn is_instruction(&self, ch: char) -> bool {
        ch == '+' || ch == '-' || ch == '<' ||
        ch == '>' || ch == '.' || ch == ',' ||
        ch == '[' || ch == ']'
    }
}
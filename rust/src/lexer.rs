
use crate::log_error;

/* --- type definitions ---------------------------------------------------- */

/// This represents the tokens that can be found in the Brainfuck source
/// program.
#[derive(PartialEq, Clone, Copy)]
pub enum Token {
    Plus,
    Minus,
    ArrowLeft,
    ArrowRight,
    Dot,
    Comma,
    BracketLeft,
    BracketRight
}

/// This represents the lexical analyzer used to retrieve tokens from the
/// Brainfuck source program.
pub struct Lexer {
    source_content: String,
    curr: usize
}

/* --- implementations ----------------------------------------------------- */

impl Lexer {
    /// Attempts to create an instance of the lexical analyzer. `Ok` on
    /// successful creation and `Err` if the source file does not exist
    /// or if the file content could not be read.
    /// 
    /// # Parameters
    /// 
    /// - `filepath` - The path to a Brainfuck source file.
    pub fn from_source(filepath: &str) -> Result<Self, &str> {
        let buf: Result<String, std::io::Error> = std::fs::read_to_string(filepath);
        if buf.is_err() {
            return Err("couldn't read file content");
        }

        Ok(Lexer {
            source_content: buf.unwrap(),
            curr: 0_usize
        })
    }

    /// Returns the next token in the source program. This will return `None`
    /// when the end of the file has been reached.
    pub fn next_token(&mut self) -> Option<Token> {
        loop {
            let ch: char = self.source_content
                .chars()
                .nth(self.curr)?;

            self.curr += 1;
            if is_bf_command(ch) {
                return match ch {
                    '+' => Some(Token::Plus),
                    '-' => Some(Token::Minus),
                    '<' => Some(Token::ArrowLeft),
                    '>' => Some(Token::ArrowRight),
                    '.' => Some(Token::Dot),
                    ',' => Some(Token::Comma),
                    '[' => Some(Token::BracketLeft),
                    ']' => Some(Token::BracketRight),
                    _=> {
                        log_error!("unknown command: {}", ch);
                        std::process::exit(0);
                    }
                }
            }
        }
    }
}

/// Checks if `ch` is a Brainfuck command and returns `true` if it is. `false`
/// otherwise.
#[inline]
fn is_bf_command(ch: char) -> bool {
    ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
    ch == '.' || ch == ',' || ch == '[' || ch == ']'
}
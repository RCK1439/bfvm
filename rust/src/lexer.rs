
/* --- type definitions ---------------------------------------------------- */

/// This represents the tokens that can be found in the Brainfuck source
/// program.
#[derive(PartialEq)]
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

/// This represents the lexical analyzer used to retrieve tokens from the
/// Brainfuck source program.
pub struct Lexer {
    src_buf: String,
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
    pub fn from_source(filepath: &str) -> Result<Self, std::io::Error> {
        Ok(Lexer {
            src_buf: std::fs::read_to_string(filepath)?,
            curr: 0usize
        })
    }

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
                        _ => Token::None // This should never happen
                    }
                }
            } else {
                return Token::EndOfFile;
            }
        }
    }

    fn next_character(&mut self) -> Option<char> {
        let ch: char = self.src_buf
            .chars()
            .nth(self.curr)?;

        self.curr += 1;
        Some(ch)
    }
}

/// Checks if `ch` is a Brainfuck command and returns `true` if it is. `false`
/// otherwise.
#[inline]
fn is_bf_command(ch: char) -> bool {
    ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
    ch == '.' || ch == ',' || ch == '[' || ch == ']'
}

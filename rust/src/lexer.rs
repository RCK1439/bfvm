
/* --- type definitions ---------------------------------------------------- */

/// This represents the tokens that can be found in the Brainfuck source
/// program.
#[derive(PartialEq, Clone, Copy)]
pub enum Token {
    Plus         = 0x2B,
    Minus        = 0x2D,
    ArrowLeft    = 0x3C,
    ArrowRight   = 0x3E,
    Dot          = 0x2E,
    Comma        = 0x2C,
    BracketLeft  = 0x5B,
    BracketRight = 0x5D
}

/// This represents the lexical analyzer used to retrieve tokens from the
/// Brainfuck source program.
pub struct Lexer {
    src_buf: String,
    curr: usize
}

/* --- implementations ----------------------------------------------------- */

impl Token {
    /// Creates a token from the character.
    /// 
    /// # Parameters
    /// 
    /// - `ch` - The character to create the token from
    /// 
    /// # Note
    /// 
    /// - This function assumes that `ch` is a valid Brainfuck command.
    /// - Before calling this function ensure that `ch` is valid.
    #[inline]
    unsafe fn from_bf_cmd(ch: char) -> Self {
        let val: u8 = ch as u8;
        std::mem::transmute(val)
    }
}

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

    /// Returns the next token in the source program. This will return `None`
    /// when the end of the file has been reached.
    pub fn next_token(&mut self) -> Option<Token> {
        loop {
            let ch: char = self.src_buf
                .chars()
                .nth(self.curr)?;

            self.curr += 1;
            if is_bf_command(ch) {
                return unsafe { Some(Token::from_bf_cmd(ch)) };
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

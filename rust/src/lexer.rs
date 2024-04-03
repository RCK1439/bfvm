use crate::log_fatal;


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

pub struct Lexer {
    source: String,
    curr: usize
}

impl Lexer {
    pub fn from_source(filepath: &str) -> Result<Self, std::io::Error> {
        Ok(Self {
            source: std::fs::read_to_string(filepath)?,
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
                        _ => {
                            log_fatal!("unknown symbol: '{ch}'");
                            std::process::exit(1);
                        }
                    };
                }
            } else {
                return Token::EndOfFile;
            }
        }
    }

    fn next_character(&mut self) -> Option<char> {
        let ch: char = self.source
            .chars()
            .nth(self.curr)?;

        self.curr += 1;
        Some(ch)
    }
}

#[inline(always)]
fn is_bf_command(ch: char) -> bool {
    ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
    ch == '.' || ch == ',' || ch == '[' || ch == ']'
}

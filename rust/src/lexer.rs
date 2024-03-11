pub struct Lexer {
    source_content: String,
    curr: usize
}

#[derive(PartialEq)]
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

impl Lexer {
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

    pub fn next_token(&mut self) -> Option<Token> {
        loop {
            let ch: Option<char> = self.source_content
                .chars()
                .nth(self.curr);

            if ch.is_none() {
                return None;
            }

            self.curr += 1;
            let character: char = ch.unwrap();

            if is_bf_command(character) {
                return match character {
                    '+' => Some(Token::Plus),
                    '-' => Some(Token::Minus),
                    '<' => Some(Token::ArrowLeft),
                    '>' => Some(Token::ArrowRight),
                    '.' => Some(Token::Dot),
                    ',' => Some(Token::Comma),
                    '[' => Some(Token::BracketLeft),
                    ']' => Some(Token::BracketRight),
                    _=> {
                        println!("bfvm: error: unknown command: {}", character);
                        std::process::exit(0);
                    }
                }
            }
        }
    }
}

#[inline]
fn is_bf_command(ch: char) -> bool {
    ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
    ch == '.' || ch == ',' || ch == '[' || ch == ']'
}
use std::io::Read;

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

pub struct LexicalAnalyzer {
    content: String,
    curr: usize
}

impl LexicalAnalyzer {
    pub fn init(filepath: &str) -> LexicalAnalyzer {
        let file = std::fs::File::open(filepath)
            .expect("failed to open file");

        let mut reader = std::io::BufReader::new(file);
        let mut data = String::new();

        reader.read_to_string(&mut data)
            .expect("failed to read data");

        return LexicalAnalyzer {
            content: data,
            curr: 0
        };
    }

    pub fn get_token(&mut self) -> Token {
        loop {
            if self.curr >= self.content.len() {
                return Token::EndOfFile;
            }

            let ch = self.content
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

    fn is_instruction(&self, ch: char) -> bool {
        return
            ch == '+' || ch == '-' || ch == '<' ||
            ch == '>' || ch == '.' || ch == ',' ||
            ch == '[' || ch == ']';
    }
}
use crate::bfc::lexer::SourcePosition;
use crate::bfc::lexer::token::Token;

use colored::{ColoredString, Colorize};

use std::fmt::Display;

/// This enum represents the different severities of errors that can occur.
#[derive(Debug, Clone)]
pub enum BFVMError {
    // Lexer errors
    NoSources,
    FailedReadingFromFile(String),
    NotABrainfuckOperator(char),

    // Parser errors
    UnexpectedToken(SourcePosition, Token),
    LoopNotClosed(SourcePosition),
    StackEmpty(SourcePosition),

    // Runtime errors
    FailedToReadByte,
    DataPointerOutOfRange,
}

const VM_NAME: &str = "bfvm";

const SEVERITY_ERROR: &str = "error";
const SEVERITY_FATAL: &str = "fatal error";

impl Display for BFVMError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        let name = ColoredString::from(VM_NAME)
            .white()
            .bold();
        
        match self {
            Self::NoSources => {
                let severity = ColoredString::from(SEVERITY_FATAL)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: no sources")
            },
            Self::FailedReadingFromFile(path) => {
                let severity = ColoredString::from(SEVERITY_FATAL)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: failed reading from file: {path}")
            }
            Self::NotABrainfuckOperator(ch) => {
                let severity = ColoredString::from(SEVERITY_FATAL)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: {ch} is not a Brainfuck operator")
            },

            Self::UnexpectedToken(pos, found) => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();
                let ln = pos.line;
                let col = pos.column;

                write!(f, "{name}:{ln}:{col}: {severity}: unexpected operator: {}", found.lexeme())
            },
            Self::LoopNotClosed(pos) => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();
                let ln = pos.line;
                let col = pos.column;

                write!(f, "{name}:{ln}:{col}: {severity}: loop not closed")
            },
            Self::StackEmpty(pos) => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();
                let ln = pos.line;
                let col = pos.column;

                write!(f, "{name}:{ln}:{col}: {severity}: stack empty")
            },

            Self::FailedToReadByte => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: failed to read byte from stdin")
            },
            Self::DataPointerOutOfRange => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: data pointer out of range")
            }
        }
    }
}

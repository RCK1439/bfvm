use colored::{ColoredString, Colorize};

use std::fmt;

/// This enum represents the different severities of errors that can occur.
#[derive(PartialEq, Debug, Clone)]
pub enum BFVMError {
    Info(String),
    Warn(String),
    Error(String),
    Fatal(String),
    InfoPos(usize, usize, String),
    WarnPos(usize, usize, String),
    ErrorPos(usize, usize, String),
    FatalPos(usize, usize, String)
}

impl fmt::Display for BFVMError {
    /// Formats the `BFVMError` according to the severity of the error.
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        const VM_NAME = "bfvm";
        
        const SEVERITY_INFO = "info";
        const SEVERITY_WARN = "warning";
        const SEVERITY_ERROR = "error";
        const SEVERITY_FATAL = "fatal error";

        let name = ColoredString::from(VM_NAME)
            .white()
            .bold();
        
        match self {
            BFVMError::Info(msg) => {
                let severity = ColoredString::from(SEVERITY_INFO)
                    .bold()
                    .blue();

                write!(f, "{name}: {severity}: {msg}")
            },
            BFVMError::Warn(msg) => {
                let severity = ColoredString::from(SEVERITY_WARN)
                    .bold()
                    .yellow();

                write!(f, "{name}: {severity}: {msg}")
            },
            BFVMError::Error(msg) => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: {msg}")
            },
            BFVMError::Fatal(msg) => {
                let severity = ColoredString::from(SEVERITY_FATAL)
                    .bold()
                    .red();

                write!(f, "{name}: {severity}: {msg}")
            }
            BFVMError::InfoPos(line, column, msg) => {
                let severity = ColoredString::from(SEVERITY_INFO)
                    .bold()
                    .blue();

                write!(f, "{name}:{line}:{column}: {severity}: {msg}")
            },
            BFVMError::WarnPos(line, column, msg) => {
                let severity = ColoredString::from(SEVERITY_WARN)
                    .bold()
                    .yellow();

                write!(f, "{name}:{line}:{column}: {severity}: {msg}")
            },
            BFVMError::ErrorPos(line, column, msg) => {
                let severity = ColoredString::from(SEVERITY_ERROR)
                    .bold()
                    .red();

                write!(f, "{name}:{line}:{column}: {severity}: {msg}")
            },
            BFVMError::FatalPos(line, column, msg) => {
                let severity = ColoredString::from(SEVERITY_FATAL)
                    .bold()
                    .red();
    
                write!(f, "{name}:{line}:{column}: {severity}: {msg}")
            },
        }
    }
}

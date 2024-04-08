use colored::{ColoredString, Colorize};
use crate::lexer::SourcePosition;

use std::fmt;

/// This struct represents an error relevant to the Brainfuck Virtual Machine.
#[derive(Debug, Clone)]
pub struct BFVMError {
    /// The error message.
    msg: String,

    /// The severity of the error.
    severity: BFVMErrSeverity,

    /// The position in the source code where the error occured.
    /// This is only relevant when `severity` = `Error`, as this suggests that
    /// it was a compilation error. 
    position: SourcePosition
}

/// This enum represents the different severities of errors that can occur.
#[derive(PartialEq, Debug, Clone)]
pub enum BFVMErrSeverity {
    Info,
    Warn,
    Error,
    Fatal
}

impl BFVMError {
    /// Creates an instance of `BFVMError`.
    /// 
    /// # Parameters
    /// 
    /// - `msg` - The error message.
    /// - `severity` - The severity of the error.
    pub fn new(msg: String, severity: BFVMErrSeverity) -> Self {
        Self {
            msg,
            severity,
            position: SourcePosition::begin()
        }
    }

    /// Creates an instance of `BFVMError` from a `SourcePosition`
    /// 
    /// # Parameters
    /// 
    /// - `position` - The position in the source code where the error occured.
    /// - `msg` - The error message.
    /// - `severity` - The severity of the error.
    pub fn from_position(position: SourcePosition, msg: String, severity: BFVMErrSeverity) -> Self {
        Self {
            msg,
            severity,
            position
        }
    }
}

impl fmt::Display for BFVMError {
    /// Formats the `BFVMError` according to the severity of the error.
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        const VM_NAME: &'static str = "bfvm";
        
        const SEVERITY_INFO: &'static str = "info";
        const SEVERITY_WARN: &'static str = "warning";
        const SEVERITY_ERROR: &'static str = "error";
        const SEVERITY_FATAL: &'static str = "fatal error";

        let name: ColoredString = ColoredString::from(VM_NAME)
            .white()
            .bold();
        let severity: ColoredString = match self.severity {
            BFVMErrSeverity::Info => ColoredString::from(SEVERITY_INFO).blue().bold(),
            BFVMErrSeverity::Warn => ColoredString::from(SEVERITY_WARN).yellow().bold(),
            BFVMErrSeverity::Error => ColoredString::from(SEVERITY_ERROR).red().bold(),
            BFVMErrSeverity::Fatal => ColoredString::from(SEVERITY_FATAL).red().bold(),
        };

        
        if self.severity == BFVMErrSeverity::Error {
            let pos_str: ColoredString = ColoredString::from(format!("{}:{}", self.position.line, self.position.column))
                .white()
                .bold();
            write!(f, "{name}:{pos_str}: {severity}: {}", self.msg)
        } else {
            write!(f, "{name}: {severity}: {}", self.msg)
        }
    }
}

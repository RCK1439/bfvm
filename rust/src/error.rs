use colored::{ColoredString, Colorize};

/// This struct represents an error relevant to the Brainfuck Virtual Machine.
#[derive(Debug, Clone)]
pub struct BFVMError {
    /// The error message.
    msg: String,

    /// The severity of the error.
    severity: BFVMErrSeverity
}

/// This enum represents the different severities of errors that can occur.
#[derive(Debug, Clone)]
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
            severity
        }
    }
}

impl std::fmt::Display for BFVMError {
    /// Formats the `BFVMError` according to the severity of the error.
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
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

        write!(f, "{name}: {severity}: {}", self.msg)
    }
}

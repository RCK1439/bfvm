#[macro_export]
macro_rules! log_info {
    ($format_str:expr $(, $args:expr)*) => {{
        use colored::{ColoredString, Colorize};

        let msg: String = format!($format_str $(, $args)*);
        let vm_name: ColoredString = ColoredString::from("bfvm").white().bold();
        let severity: ColoredString = ColoredString::from("info").blue().bold();

        println!("{}: {}: {}", vm_name, severity, msg);
    }};
}

#[macro_export]
macro_rules! log_warn {
    ($format_str:expr $(, $args:expr)*) => {{
        use colored::{ColoredString, Colorize};

        let msg: String = format!($format_str $(, $args)*);
        let vm_name: ColoredString = ColoredString::from("bfvm").white().bold();
        let severity: ColoredString = ColoredString::from("warning").yellow().bold();

        println!("{}: {}: {}", vm_name, severity, msg);
    }};
}

#[macro_export]
macro_rules! log_error {
    ($format_str:expr $(, $args:expr)*) => {{
        use colored::{ColoredString, Colorize};

        let msg: String = format!($format_str $(, $args)*);
        let vm_name: ColoredString = ColoredString::from("bfvm").white().bold();
        let severity: ColoredString = ColoredString::from("error").red().bold();

        println!("{}: {}: {}", vm_name, severity, msg);
    }};
}

#[macro_export]
macro_rules! log_fatal {
    ($format_str:expr $(, $args:expr)*) => {{
        use colored::{ColoredString, Colorize};

        let msg: String = format!($format_str $(, $args)*);
        let vm_name: ColoredString = ColoredString::from("bfvm").white().bold();
        let severity: ColoredString = ColoredString::from("fatal error").red().bold();

        println!("{}: {}: {}", vm_name, severity, msg);
    }};
}
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args()
        .collect();

    bfvm::run(args)
}

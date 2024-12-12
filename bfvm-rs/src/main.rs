use std::process::ExitCode;

/// Entry point of the application. This will run the provided Brainfuck
/// program by first compiling the source code and then sending the byte code
/// to the virtual machine to be interpreted
/// 
/// # Arguments
/// 
/// To run the program: `bfvm path/to/brainfuck.b`
fn main() -> ExitCode {
    let args = std::env::args()
        .collect();

    if let Err(err) = bfvm::run(args) {
        println!("{err}");
        return ExitCode::FAILURE;
    }

    ExitCode::SUCCESS
}

use std::process;
use std::env;

/// Entry point of the application. This will run the provided Brainfuck
/// program by first compiling the source code and then sending the byte code
/// to the virtual machine to be interpreted.
/// 
/// # Arguments
/// To run the program: `bfvm path/to/brainfuck.b`
fn main() -> process::ExitCode {
    let args: Vec<String> = env::args()
        .collect();

    if let Err(err) = bfvm::run(args) {
        println!("{err}");
        return process::ExitCode::FAILURE;
    }

    process::ExitCode::SUCCESS
}

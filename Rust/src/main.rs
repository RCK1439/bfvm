mod bfvm;
mod bfc;
mod lexer;

use bfvm::VirtualMachine;

/// Main routine for the BFVM developed in the Rust Programming Language.
fn main() {
    let args: Vec<String> = std::env::args()
        .collect();

    if args.len() < 2 {
        eprintln!("bfvm: fatal error: no sources");
        std::process::exit(1);
    }

    let mut vm: VirtualMachine = bfvm::VirtualMachine::new(&args[1]);
    vm.run();
}

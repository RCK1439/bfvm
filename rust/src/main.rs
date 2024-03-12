use bfc::{OpCode, Compiler};
use bfvm::VirtualMachine;

mod bfc;
mod bfvm;
mod lexer;

/// The entry point of BFVM. To run the BFVM from the command line the
/// arguments must look as follows: `./bfvm <filepath>`
fn main() {
    let args: Vec<String> = std::env::args()
        .collect();

    if args.len() < 2 {
        // TODO: Implement error emitter.
        println!("bfvm: error: no sources");
        std::process::exit(0);
    }

    let mut bfc: Compiler = Compiler::from_source(&args[1]);
    let asm: Vec<OpCode> = bfc.compile();
    
    let mut vm: VirtualMachine = VirtualMachine::from_asm(asm);
    vm.run();
}

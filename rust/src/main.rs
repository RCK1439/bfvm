use bfc::OpCode;
use bfvm::VirtualMachine;

mod bfc;
mod bfvm;
mod lexer;

fn main() {
    let args: Vec<String> = std::env::args()
        .collect();

    if args.len() < 2 {
        // TODO: Implement error emitter.
        println!("bfvm: error: no sources");
        std::process::exit(0);
    }

    let code: Vec<OpCode> = bfc::compile(&args[1]);
    let mut vm: VirtualMachine = VirtualMachine::from_asm(code);
    vm.run();
}

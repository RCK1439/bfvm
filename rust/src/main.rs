use bfvm::VirtualMachine;

mod bfc;
mod bfvm;
mod error;
mod lexer;

fn main() {
    let args: Vec<String> = std::env::args()
        .collect();

    if args.len() < 2 {
        log_fatal!("no sources");
        std::process::exit(1);
    }

    let mut vm: VirtualMachine = VirtualMachine::init(&args[1]);
    vm.run();
}

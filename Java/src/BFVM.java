public class BFVM {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("bfvm: error: no sources");
            System.exit(1);
        }

        final VirtualMachine vm = new VirtualMachine(args[0]);
        vm.run();
    }
}
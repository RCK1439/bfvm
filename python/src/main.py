import sys
from vm import VirtualMachine

def main():
    if len(sys.argv) < 2:
        print("bfvm: error: no sources")
        exit(1)

    vm: VirtualMachine = VirtualMachine(sys.argv[1])
    vm.run()

if __name__ == "__main__":
    main()
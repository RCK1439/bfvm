import sys

from virtual_machine import VirtualMachine

def main():
    if len(sys.argv) < 2:
        print('not enough arguments')
        return
    
    vm = VirtualMachine(sys.argv[1])
    vm.run()

if __name__ == '__main__':
    main()

import java.io.IOException;
import java.util.ArrayList;

public class VirtualMachine {
    
    byte[] dataArray;
    int dataPtr;
    ArrayList<ByteCode> code;
    int instructionPtr;
    
    public VirtualMachine(String filePath) {
        final Compiler bfc = new Compiler(filePath);
        
        this.dataArray = new byte[30000];
        this.dataPtr = 0;
        this.code = bfc.compile();
        this.instructionPtr = 0;
    }

    public void run() {
        ByteCode c = code.get(instructionPtr);
        while (c.code != OpCode.END) {
            switch (c.code) {
                case ADDB: addByte((byte) c.operand); break;
                case SUBB: subByte((byte) c.operand); break;
                case ADDP: addPointer(c.operand); break;
                case SUBP: subPointer(c.operand); break;
                case WRITE: write(); break;
                case READ: read(); break;
                case JZ: jumpZero(c.operand); break;
                case JMP: jump(c.operand); break;
                default: break; // This should never happen
            }

            c = code.get(instructionPtr);
        }
    }

    private void addByte(byte val) {
        dataArray[dataPtr] += val;
        instructionPtr++;
    }

    private void subByte(byte val) {
        dataArray[dataPtr] -= val;
        instructionPtr++;
    }

    private void addPointer(int offset) {
        dataPtr += offset;
        if (dataPtr > 30000) {
            System.err.println("bfvm: error: data pointer out of range");
            System.exit(1);
        }

        instructionPtr++;
    }

    private void subPointer(int offset) {
        dataPtr -= offset;
        if (dataPtr > 30000) {
            System.err.println("bfvm: error: data pointer out of range");
            System.exit(1);
        }

        instructionPtr++;
    }

    private void write() {
        System.out.print((char) dataArray[dataPtr]);
        instructionPtr++;
    }

    private void read() {
        try {
            dataArray[dataPtr] = (byte) System.in.read();
        } catch (IOException e) {
            System.err.println("bfvm: error: failed to read byte");
            System.exit(1);
        }
        instructionPtr++;
    }

    private void jumpZero(int line) {
        if (dataArray[dataPtr] != 0x00) {
            instructionPtr++;
        } else {
            instructionPtr = line;
        }
    }

    private void jump(int line) {
        instructionPtr = line;
    }
}

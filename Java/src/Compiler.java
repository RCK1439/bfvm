
import java.util.ArrayList;
import java.util.Stack;

public class Compiler {
    
    private Lexer lexer;
    private int currentLine;
    private Token currentToken;

    public Compiler(String filePath) {
        this.lexer = new Lexer(filePath);
        this.currentLine = 0;
        this.currentToken = Token.NONE;
    }

    public ArrayList<ByteCode> compile() {
        final ArrayList<ByteCode> code = new ArrayList<ByteCode>();

        currentToken = lexer.nextToken();
        while (currentToken != Token.END_OF_FILE) {
            switch (currentToken) {
                case PLUS: parseAddByte(code); break;
                case MINUS: parseSubByte(code); break;
                case ARROW_LEFT: parseSubPtr(code); break;
                case ARROW_RIGHT: parseAddPtr(code); break;
                case DOT: parseWrite(code); break;
                case COMMA: parseRead(code); break;
                case BRACKET_LEFT: parseConditional(code); break;
                default: {
                    System.err.println("bfvm: error: unexpected token");
                    System.exit(1);
                } break;
            }
        }

        code.add(new ByteCode(OpCode.END, 0));
        lexer.close();
        
        return code;
    }

    private void parseAddByte(ArrayList<ByteCode> code) {
        int val = 0;

        while (currentToken == Token.PLUS) {
            val++;
            currentToken = lexer.nextToken();
        }

        code.add(new ByteCode(OpCode.ADDB, val));
        currentLine++;
    }

    private void parseSubByte(ArrayList<ByteCode> code) {
        int val = 0;

        while (currentToken == Token.MINUS) {
            val++;
            currentToken = lexer.nextToken();
        }

        code.add(new ByteCode(OpCode.SUBB, val));
        currentLine++;
    }

    private void parseAddPtr(ArrayList<ByteCode> code) {
        int offset = 0;

        while (currentToken == Token.ARROW_RIGHT) {
            offset++;
            currentToken = lexer.nextToken();
        }

        code.add(new ByteCode(OpCode.ADDP, offset));
        currentLine++;
    }

    private void parseSubPtr(ArrayList<ByteCode> code) {
        int offset = 0;

        while (currentToken == Token.ARROW_LEFT) {
            offset++;
            currentToken = lexer.nextToken();
        }

        code.add(new ByteCode(OpCode.SUBP, offset));
        currentLine++;
    }

    private void parseWrite(ArrayList<ByteCode> code) {
        code.add(new ByteCode(OpCode.WRITE, 0));
        currentToken = lexer.nextToken();

        currentLine++;
    }

    private void parseRead(ArrayList<ByteCode> code) {
        code.add(new ByteCode(OpCode.READ, 0));
        currentToken = lexer.nextToken();

        currentLine++;
    }

    private void parseConditional(ArrayList<ByteCode> code) {
        final Stack<Integer> braces = new Stack<Integer>();

        braces.push(currentLine++);

        code.add(new ByteCode(OpCode.JZ, 0));
        currentToken = lexer.nextToken();
        while (!braces.empty()) {
            if (currentToken == Token.END_OF_FILE) {
                System.err.println("bfvm: error: unbalanced braces");
                System.exit(1);
            }

            switch (currentToken) {
                case PLUS: parseAddByte(code); break;
                case MINUS: parseSubByte(code); break;
                case ARROW_LEFT: parseSubPtr(code); break;
                case ARROW_RIGHT: parseAddPtr(code); break;
                case DOT: parseWrite(code); break;
                case COMMA: parseRead(code); break;
                case BRACKET_LEFT: {
                    braces.push(currentLine++);

                    code.add(new ByteCode(OpCode.JZ, 0));
                    currentToken = lexer.nextToken();
                } break;
                case BRACKET_RIGHT: {
                    final int open = braces.pop();
                    final ByteCode openingBrace = code.get(open);
                    
                    openingBrace.operand = ++currentLine;
                    code.add(new ByteCode(OpCode.JMP, open));
                    
                    currentToken = lexer.nextToken();
                } break;
                default: {
                    System.err.println("bfvm: error: unexpected token");
                    System.exit(1);
                } break;
            }
        }
    }
}

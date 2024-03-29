import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Lexer {
    
    private BufferedReader reader;
    
    public Lexer(String filePath) {
        try {
            this.reader = new BufferedReader(new FileReader(filePath));
        } catch (IOException e) {
            System.err.println("bfvm: error: couldn't open file");
            System.exit(1);
        }
    }

    public void close() {
        try {
            reader.close();
        } catch (IOException e) {
            System.err.println("bfvm: error: failed closing resources of lexer");
            System.exit(1);
        }
    }

    public Token nextToken() {
        while (true) {
            final char ch = nextCharacter();
            if (isBFCommand(ch)) {
                switch (ch) {
                    case '+': return Token.PLUS;
                    case '-': return Token.MINUS;
                    case '<': return Token.ARROW_LEFT;
                    case '>': return Token.ARROW_RIGHT;
                    case '.': return Token.DOT;
                    case ',': return Token.COMMA;
                    case '[': return Token.BRACKET_LEFT;
                    case ']': return Token.BRACKET_RIGHT;
                    default: return Token.NONE; // This should never happen.
                }
            } else if (ch == 0xFFFF) {
                return Token.END_OF_FILE;
            }
        }
    }

    private char nextCharacter() {
        try {
            return (char) reader.read();
        } catch (IOException e) {
            System.err.println("bfvm: error: failed to read character from file");
            System.exit(1);
        }

        return 0x00;
    }

    private boolean isBFCommand(char ch) {
        return ch == '+' || ch == '-' || ch == '<' || ch == '>' ||
            ch == '.' || ch == ',' || ch == '[' || ch == ']';
    }
}

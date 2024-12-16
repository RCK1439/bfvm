package bfc

import "core:os"
import "core:strings"
import "core:fmt"

Token :: enum {
    NONE = 0,
    PLUS,
    MINUS,
    ARROW_LEFT,
    ARROW_RIGHT,
    DOT,
    COMMA,
    BRACKET_LEFT,
    BRACKET_RIGHT,
    END_OF_FILE,
}

Source_Position :: struct {
    line:   u64,
    column: u64
}

Lexer :: struct {
    source_content: string,
    curr_idx:       u64,
    position:       Source_Position
}

create_lexer :: proc(filepath: string) -> Lexer {
    data, ok := os.read_entire_file_from_filename(filepath)
    defer delete(data)
    
    if !ok {
        fmt.printf("Failed to open file: %s\n", filepath)
    }

    content := strings.clone_from_bytes(data)
    return Lexer {
        content,
        0,
        Source_Position {
            1,
            0
        }
    }
}

close_lexer :: proc(lexer: ^Lexer) {
    delete(lexer.source_content)
}

get_token :: proc(lexer: ^Lexer) -> Token {
    size := cast(u64)len(lexer.source_content)

    for {
        if lexer.curr_idx >= size {
            return Token.END_OF_FILE
        }
        
        ch := lexer.source_content[lexer.curr_idx]
        lexer.curr_idx += 1

        if !is_brainfuck_command(ch) {
            continue
        }

        switch ch {
            case '+': return Token.PLUS
            case '-': return Token.MINUS
            case '<': return Token.ARROW_LEFT
            case '>': return Token.ARROW_RIGHT
            case '.': return Token.DOT
            case ',': return Token.COMMA
            case '[': return Token.BRACKET_LEFT
            case ']': return Token.BRACKET_RIGHT
        }
    }
}

is_brainfuck_command :: proc(ch: u8) -> bool {
    return ch == '-' || ch == '+' || ch == '.' || ch == ',' ||
    ch == '[' || ch == ']' || ch == '<' || ch == '>'
}

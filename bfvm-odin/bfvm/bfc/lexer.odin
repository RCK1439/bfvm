package bfc

import "core:os"
import "core:strings"
import "core:fmt"

Token :: enum {
    None = 0,
    Plus,
    Minus,
    Arrow_Left,
    Arrow_Right,
    Dot,
    Comma,
    Bracket_Left,
    Bracket_Right,
    End_Of_File,
}

Source_Position :: struct {
    line: u64,
    column: u64
}

Lexer :: struct {
    source_content: string,
    curr_idx: u64,
    position: Source_Position
}

create_lexer :: proc(filepath: string) -> Lexer {
    data, ok := os.read_entire_file(filepath, context.allocator)
    defer delete(data, context.allocator)
    
    if !ok {
        fmt.printf("Failed to open file: %s\n", filepath)
    }

    content := string(data)
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
    delete(lexer.source_content, context.allocator)
}

get_token :: proc(lexer: ^Lexer) -> Token {
    size := cast(u64)len(lexer.source_content)

    for {
        if lexer.curr_idx >= size {
            return Token.End_Of_File
        }
        
        ch := lexer.source_content[lexer.curr_idx]
        lexer.curr_idx += 1

        if !is_brainfuck_command(ch) {
            continue
        }

        switch ch {
            case '+': return Token.Plus
            case '-': return Token.Minus
            case '<': return Token.Arrow_Left
            case '>': return Token.Arrow_Right
            case '.': return Token.Dot
            case ',': return Token.Comma
            case '[': return Token.Bracket_Left
            case ']': return Token.Bracket_Right
        }
    }
}

is_brainfuck_command :: proc(ch: u8) -> bool {
    return ch == '-' || ch == '+' || ch == '.' || ch == ',' ||
    ch == '[' || ch == ']' || ch == '<' || ch == '>'
}

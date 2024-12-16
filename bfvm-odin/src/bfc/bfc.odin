package bfc

Op_Code :: enum {
    ADDB,
    SUBB,
    ADDP,
    SUBP,
    JMP,
    JZ,
    READ,
    WRITE,
    END,
}

Operand :: union {
    u8,
    u16,
    u64
}

Byte_Code :: struct {
    opcode:  Op_Code,
    operand: Operand
}

create_bytecode :: proc(opcode: Op_Code, operand: Operand) -> Byte_Code {
    return Byte_Code {
        opcode,
        operand
    }
}

Compiler :: struct {
    lexer:      Lexer,
    curr_token: Token,
    curr_line:  u64,
}

create_compiler :: proc(filepath: string) -> Compiler {
    return Compiler {
        create_lexer(filepath),
        Token.NONE,
        0
    }
}

close_compiler :: proc(compiler: ^Compiler) {
    close_lexer(&compiler.lexer)
}

compile :: proc(compiler: ^Compiler) -> [dynamic]Byte_Code {
    code := make_dynamic_array([dynamic]Byte_Code, context.allocator)

    compiler.curr_token = get_token(&compiler.lexer)
    for compiler.curr_token != Token.END_OF_FILE {
        #partial switch compiler.curr_token {
            case .PLUS:
                parse_add_byte(compiler, &code)
                break
            case .MINUS:
                parse_sub_byte(compiler, &code)
                break
            case .ARROW_LEFT:
                parse_sub_ptr(compiler, &code)
                break
            case .ARROW_RIGHT:
                parse_add_ptr(compiler, &code)
                break
            case .BRACKET_LEFT:
                parse_conditional(compiler, &code)
                break
            case .DOT:
                parse_write(compiler, &code)
                break
            case .COMMA:
                parse_read(compiler, &code)
                break
        }
    }

    append_elem(&code, create_bytecode(Op_Code.END, cast(u8)0))
    compiler.curr_line += 1

    return code
}

parse_add_byte :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.PLUS {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.ADDB, cast(u8)offset))
    compiler.curr_line += 1
}

parse_sub_byte :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.MINUS {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.SUBB, cast(u8)offset))
    compiler.curr_line += 1
}

parse_add_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.ARROW_RIGHT {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.ADDP, cast(u16)offset))
    compiler.curr_line += 1
}

parse_sub_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.ARROW_LEFT {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.SUBP, cast(u16)offset))
    compiler.curr_line += 1
}

parse_read :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    append_elem(code, create_bytecode(Op_Code.READ, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)
}

parse_write :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    append_elem(code, create_bytecode(Op_Code.WRITE, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)
}

parse_conditional :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    open_pos := compiler.curr_line

    append_elem(code, create_bytecode(Op_Code.JZ, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)

    for compiler.curr_token != Token.BRACKET_RIGHT {
        #partial switch compiler.curr_token {
	        case .PLUS:
                parse_add_byte(compiler, code)
                break
	        case .MINUS:
                parse_sub_byte(compiler, code)
                break
	        case .ARROW_LEFT:
                parse_sub_ptr(compiler, code)
                break
	        case .ARROW_RIGHT:
                parse_add_ptr(compiler, code)
                break
	        case .DOT:
                parse_write(compiler, code)
                break
	        case .COMMA:
                parse_read(compiler, code)
                break
	        case .BRACKET_LEFT:
                parse_conditional(compiler, code)
                break
	        case .END_OF_FILE:
                assert(false, "end of file reached whilst parsing conditional")
        }
    }

    compiler.curr_token = get_token(&compiler.lexer)
    code[open_pos].operand = compiler.curr_line + 1;

    append_elem(code, create_bytecode(Op_Code.JMP, open_pos))
    compiler.curr_line += 1
}
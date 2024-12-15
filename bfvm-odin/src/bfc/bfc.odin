package bfc

Op_Code :: enum {
    Addb,
    Subb,
    Addp,
    Subp,
    Jmp,
    Jz,
    Read,
    Write,
    End,
}

Operand :: union {
    u8,
    u16,
    u64
}

Byte_Code :: struct {
    opcode: Op_Code,
    operand: Operand
}

create_bytecode :: proc(opcode: Op_Code, operand: Operand) -> Byte_Code {
    return Byte_Code {
        opcode,
        operand
    }
}

Compiler :: struct {
    lexer: Lexer,
    curr_token: Token,
    curr_line: u64,
}

create_compiler :: proc(filepath: string) -> Compiler {
    return Compiler {
        create_lexer(filepath),
        Token.None,
        0
    }
}

close_compiler :: proc(compiler: ^Compiler) {
    close_lexer(&compiler.lexer)
}

compile :: proc(compiler: ^Compiler) -> [dynamic]Byte_Code {
    code := make_dynamic_array([dynamic]Byte_Code, context.allocator)

    compiler.curr_token = get_token(&compiler.lexer)
    for compiler.curr_token != Token.End_Of_File {
        #partial switch compiler.curr_token {
            case .Plus:
                parse_add_byte(compiler, &code)
                break
            case .Minus:
                parse_sub_byte(compiler, &code)
                break
            case .Arrow_Left:
                parse_sub_ptr(compiler, &code)
                break
            case .Arrow_Right:
                parse_add_ptr(compiler, &code)
                break
            case .Bracket_Left:
                parse_conditional(compiler, &code)
                break
            case .Dot:
                parse_write(compiler, &code)
                break
            case .Comma:
                parse_read(compiler, &code)
                break
        }
    }

    append_elem(&code, create_bytecode(Op_Code.End, cast(u8)0))
    compiler.curr_line += 1

    return code
}

parse_add_byte :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.Plus {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.Addb, cast(u8)offset))
    compiler.curr_line += 1
}

parse_sub_byte :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.Minus {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.Subb, cast(u8)offset))
    compiler.curr_line += 1
}

parse_add_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.Arrow_Right {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.Addp, cast(u16)offset))
    compiler.curr_line += 1
}

parse_sub_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    offset := 0

    for compiler.curr_token == Token.Arrow_Left {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    append_elem(code, create_bytecode(Op_Code.Subp, cast(u16)offset))
    compiler.curr_line += 1
}

parse_read :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    append_elem(code, create_bytecode(Op_Code.Read, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)
}

parse_write :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    append_elem(code, create_bytecode(Op_Code.Write, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)
}

parse_conditional :: proc(compiler: ^Compiler, code: ^[dynamic]Byte_Code) {
    open_pos := compiler.curr_line

    append_elem(code, create_bytecode(Op_Code.Jz, cast(u8)0))
    compiler.curr_line += 1

    compiler.curr_token = get_token(&compiler.lexer)

    for compiler.curr_token != Token.Bracket_Right {
        #partial switch compiler.curr_token {
	        case .Plus:
                parse_add_byte(compiler, code)
                break
	        case .Minus:
                parse_sub_byte(compiler, code)
                break
	        case .Arrow_Left:
                parse_sub_ptr(compiler, code)
                break
	        case .Arrow_Right:
                parse_add_ptr(compiler, code)
                break
	        case .Dot:
                parse_write(compiler, code)
                break
	        case .Comma:
                parse_read(compiler, code)
                break
	        case .Bracket_Left:
                parse_conditional(compiler, code)
                break
	        case .End_Of_File:
                assert(false, "end of file reached whilst parsing conditional")
        }
    }

    compiler.curr_token = get_token(&compiler.lexer)
    code[open_pos].operand = compiler.curr_line + 1;

    append_elem(code, create_bytecode(Op_Code.Jmp, open_pos))
    compiler.curr_line += 1
}
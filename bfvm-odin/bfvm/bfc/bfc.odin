package bfc

OpCode :: enum {
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

ByteCode :: struct {
    opcode: OpCode,
    operand: Operand
}

create_bytecode :: proc(opcode: OpCode, operand: Operand) -> ByteCode {
    return ByteCode {
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
        Token.NONE,
        0
    }
}

close_compiler :: proc(compiler: ^Compiler) {
    close_lexer(&compiler.lexer)
}

compile :: proc(compiler: ^Compiler) -> [dynamic]ByteCode {
    code := make_dynamic_array([dynamic]ByteCode, context.allocator)

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
                parse_add_ptr(compiler, &code)
                break
            case .ARROW_RIGHT:
                parse_sub_ptr(compiler, &code)
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

    return code
}

parse_add_byte :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    offset := 0

    for compiler.curr_token == Token.PLUS {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    bytecode := create_bytecode(OpCode.ADDB, cast(u8)offset)
    append_elem(code, bytecode)

    compiler.curr_line += 1
}

parse_sub_byte :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    offset := 0

    for compiler.curr_token == Token.MINUS {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    bytecode := create_bytecode(OpCode.SUBB, cast(u8)offset)
    append_elem(code, bytecode)

    compiler.curr_line += 1
}

parse_add_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    offset := 0

    for compiler.curr_token == Token.ARROW_RIGHT {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    bytecode := create_bytecode(OpCode.ADDP, cast(u16)offset)
    append_elem(code, bytecode)

    compiler.curr_line += 1
}

parse_sub_ptr :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    offset := 0

    for compiler.curr_token == Token.ARROW_LEFT {
        offset += 1
        compiler.curr_token = get_token(&compiler.lexer)
    }

    bytecode := create_bytecode(OpCode.SUBP, cast(u16)offset)
    append_elem(code, bytecode)

    compiler.curr_line += 1
}

parse_read :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    bytecode := create_bytecode(OpCode.READ, cast(u8)0)
    append_elem(code, bytecode)

    compiler.curr_token = get_token(&compiler.lexer)
    compiler.curr_line += 1
}

parse_write :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    bytecode := create_bytecode(OpCode.WRITE, cast(u8)0)
    append_elem(code, bytecode)

    compiler.curr_token = get_token(&compiler.lexer)
    compiler.curr_line += 1
}

parse_conditional :: proc(compiler: ^Compiler, code: ^[dynamic]ByteCode) {
    append_elem(code, create_bytecode(OpCode.JZ, cast(u8)0))

    compiler.curr_token = get_token(&compiler.lexer)
    compiler.curr_line += 1

    opening_line := compiler.curr_line - 1;
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
	        case .BRACKET_RIGHT:
                compiler.curr_token = get_token(&compiler.lexer)
                compiler.curr_line += 1

                code[opening_line].operand = compiler.curr_line

                bytecode := create_bytecode(OpCode.JMP, opening_line)
                append_elem(code, bytecode)

                break
	        case .END_OF_FILE:
                // TODO: Error here
                break
        }
    }
}

from dataclasses import dataclass
from enum import Enum

from lex import BFLexer
from lex import Token

class OpCode(Enum):
    NONE = 0
    ADDB = 1
    SUBB = 2
    ADDP = 3
    SUBP = 4
    WRITE = 5
    READ = 6
    JZ = 7
    JMP = 8
    END = 9


@dataclass
class ByteCode:
    op: OpCode
    val: int


class BFCompiler:
    __lexer: BFLexer
    __token: Token
    __currln: int

    def __init__(self, filepath: str):
        self.__lexer = BFLexer(filepath)
        self.__token = Token.NONE
        self.__currln = 0

    def compile(self) -> list[ByteCode]:
        code: list[ByteCode] = []
        
        self.__token = self.__lexer.next_token()
        while self.__token != Token.END_OF_FILE:
            match self.__token:
                case Token.PLUS: self.__parse_add_byte(code)
                case Token.MINUS: self.__parse_sub_byte(code)
                case Token.ARROW_LEFT: self.__parse_sub_ptr(code)
                case Token.ARROW_RIGHT: self.__parse_add_ptr(code)
                case Token.DOT: self.__parse_write(code)
                case Token.COMMA: self.__parse_read(code)
                case Token.BRACKET_LEFT: self.__parse_conditional(code)
                case _:
                    print("bfvm: error: unexpected token: \']\'")
                    exit(1)

        code.append(ByteCode(OpCode.END, 0))
        return code

    def __parse_add_byte(self, code: list[ByteCode]):
        val: int = 0

        while self.__token == Token.PLUS:
            val += 1
            self.__token = self.__lexer.next_token()
        
        code.append(ByteCode(OpCode.ADDB, val))
        self.__currln += 1

    def __parse_sub_byte(self, code: list[ByteCode]):
        val: int = 0

        while self.__token == Token.MINUS:
            val += 1
            self.__token = self.__lexer.next_token()
        
        code.append(ByteCode(OpCode.SUBB, val))
        self.__currln += 1

    def __parse_add_ptr(self, code: list[ByteCode]):
        offset: int = 0

        while self.__token == Token.ARROW_RIGHT:
            offset += 1
            self.__token = self.__lexer.next_token()
        
        code.append(ByteCode(OpCode.ADDP, offset))
        self.__currln += 1

    def __parse_sub_ptr(self, code: list[ByteCode]):
        offset: int = 0

        while self.__token == Token.ARROW_LEFT:
            offset += 1
            self.__token = self.__lexer.next_token()
        
        code.append(ByteCode(OpCode.SUBP, offset))
        self.__currln += 1

    def __parse_write(self, code: list[ByteCode]):
        code.append(ByteCode(OpCode.WRITE, 0))
        self.__token = self.__lexer.next_token()
        self.__currln += 1

    def __parse_read(self, code: list[ByteCode]):
        code.append(ByteCode(OpCode.READ, 0))
        self.__token = self.__lexer.next_token()
        self.__currln += 1

    def __parse_conditional(self, code: list[ByteCode]):
        braces: list[int] = []

        braces.append(self.__currln)
        self.__currln += 1

        code.append(ByteCode(OpCode.JZ, 0))
        self.__token = self.__lexer.next_token()
        while braces:
            if self.__token == Token.END_OF_FILE:
                print("bfvm: error: unbalanced braces")
                exit(1)

            match self.__token:
                case Token.PLUS: self.__parse_add_byte(code)
                case Token.MINUS: self.__parse_sub_byte(code)
                case Token.ARROW_LEFT: self.__parse_sub_ptr(code)
                case Token.ARROW_RIGHT: self.__parse_add_ptr(code)
                case Token.DOT: self.__parse_write(code)
                case Token.COMMA: self.__parse_read(code)
                case Token.BRACKET_LEFT:
                    braces.append(self.__currln)
                    self.__currln += 1
                    
                    code.append(ByteCode(OpCode.JZ, 0))
                    self.__token = self.__lexer.next_token()
                case Token.BRACKET_RIGHT:
                    open: int = braces.pop()

                    self.__currln += 1
                    code[open].val = self.__currln

                    code.append(ByteCode(OpCode.JMP, open))
                    self.__token = self.__lexer.next_token()
                case _:
                    print("bfvm: error: unreachable code")
                    exit(1)
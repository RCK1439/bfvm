from enum import Enum
from dataclasses import dataclass

from .lexer import Lexer, Token

class OpCode(Enum):
    ADDB = 0,
    SUBB = 1,
    ADDP = 2,
    SUBP = 3,
    JMP = 4,
    JZ = 5,
    WRITE = 6,
    READ = 7,
    END = 8,

@dataclass
class ByteCode:
    opcode: OpCode
    value: int

class Compiler:
    _lexer: Lexer
    _token: Token
    _currln: int

    def __init__(self, filepath: str):
        self._lexer = Lexer(filepath)
        self._token = Token.NONE
        self._currln = 0

    def compile(self) -> list[ByteCode]:
        byte_code = []

        self._token = self._lexer.next_token()
        self._parse_program(byte_code)

        return byte_code

    def _parse_program(self, byte_code: list[ByteCode]):
        while self._token != Token.END_OF_FILE:
            match self._token:
                case Token.PLUS: self._parse_add_byte(byte_code)
                case Token.MINUS: self._parse_sub_byte(byte_code)
                case Token.LEFT_ARROW: self._parse_sub_ptr(byte_code)
                case Token.RIGHT_ARROW: self._parse_add_ptr(byte_code)
                case Token.LEFT_BRACKET: self._parse_conditional(byte_code)
                case Token.DOT: self._parse_write(byte_code)
                case Token.COMMA: self._parse_read(byte_code)
                case _: assert False, f'unexpected token {self._token}'

        byte_code.append(ByteCode(opcode=OpCode.END, value=0))
    
    def _parse_add_byte(self, byte_code: list[ByteCode]):
        self._parse_chain(Token.PLUS, OpCode.ADDB, byte_code)

    def _parse_sub_byte(self, byte_code: list[ByteCode]):
        self._parse_chain(Token.MINUS, OpCode.SUBB, byte_code)

    def _parse_sub_ptr(self, byte_code: list[ByteCode]):
        self._parse_chain(Token.LEFT_ARROW, OpCode.SUBP, byte_code)

    def _parse_add_ptr(self, byte_code: list[ByteCode]):
        self._parse_chain(Token.RIGHT_ARROW, OpCode.ADDP, byte_code)

    def _parse_conditional(self, byte_code: list[ByteCode]):
        braces = [self._currln]
        
        byte_code.append(ByteCode(OpCode.JZ, 0))
        self._currln += 1

        self._token = self._lexer.next_token()
        while braces.__len__() != 0:
            match self._token:
                case Token.PLUS: self._parse_add_byte(byte_code)
                case Token.MINUS: self._parse_sub_byte(byte_code)
                case Token.LEFT_ARROW: self._parse_sub_ptr(byte_code)
                case Token.RIGHT_ARROW: self._parse_add_ptr(byte_code)
                case Token.LEFT_BRACKET:
                    braces.append(self._currln)
                    self._currln += 1

                    byte_code.append(ByteCode(OpCode.JZ, 0))
                    self._token = self._lexer.next_token()
                case Token.RIGHT_BRACKET:
                    open_brace = braces.pop()
                    self._currln += 1

                    byte_code[open_brace].value = self._currln
                    byte_code.append(ByteCode(OpCode.JMP, open_brace))

                    self._token = self._lexer.next_token()
                case Token.DOT: self._parse_write(byte_code)
                case Token.COMMA: self._parse_read(byte_code)
                case _: assert False, f'unexpected token {self._token}'

    def _parse_write(self, byte_code: list[ByteCode]):
        self._token = self._lexer.next_token()
        
        byte_code.append(ByteCode(OpCode.WRITE, value=0))
        self._currln += 1

    def _parse_read(self, byte_code: list[ByteCode]):
        self._token = self._lexer.next_token()
        
        byte_code.append(ByteCode(OpCode.READ, value=0))
        self._currln += 1

    def _parse_chain(self, token: Token, instr: OpCode, byte_code: list[ByteCode]):
        value = 0

        while self._token == token:
            value += 1
            self._token = self._lexer.next_token()

        byte_code.append(ByteCode(instr, value))
        self._currln += 1

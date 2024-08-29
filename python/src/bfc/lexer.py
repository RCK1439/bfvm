from enum import Enum

class Token(Enum):
    NONE = 0,
    PLUS = 1,
    MINUS = 2,
    LEFT_ARROW = 3,
    RIGHT_ARROW = 4,
    LEFT_BRACKET = 5,
    RIGHT_BRACKET = 6,
    DOT = 7,
    COMMA = 8,
    END_OF_FILE = 9

class Lexer:
    _source: str
    _read_pos: int
    
    def __init__(self, filepath: str):
        with open(filepath, 'r') as file:
            source = file.read()
            self._source = source

        self._read_pos = 0

    def next_token(self) -> Token:
        if self._read_pos >= self._source.__len__():
                return Token.END_OF_FILE
        
        curr = self._source[self._read_pos]
        
        while not self._is_bf_char(curr):
            self._read_pos += 1
            if self._read_pos >= self._source.__len__():
                return Token.END_OF_FILE

            curr = self._source[self._read_pos]
        
        self._read_pos += 1
        match curr:
            case '+': return Token.PLUS
            case '-': return Token.MINUS
            case '<': return Token.LEFT_ARROW
            case '>': return Token.RIGHT_ARROW
            case '[': return Token.LEFT_BRACKET
            case ']': return Token.RIGHT_BRACKET
            case '.': return Token.DOT
            case ',': return Token.COMMA
            case _: return Token.NONE

    def _is_bf_char(self, ch: str) -> bool:
        return ch == '+' or ch == '-' or ch == '<' or ch == '>' or ch == '[' or ch == ']' or ch == '.' or ch == ','

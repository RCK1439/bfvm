
from enum import Enum

class Token(Enum):
    NONE = 0
    PLUS = 0x2B
    MINUS = 0x2D
    ARROW_LEFT = 0x3C
    ARROW_RIGHT = 0x3E
    DOT = 0x2E
    COMMA = 0x2C
    BRACKET_LEFT = 0x5B
    BRACKET_RIGHT = 0x5D
    END_OF_FILE = -1

class BFLexer:
    __content: str
    __curr: int
    __size: int

    def __init__(self, filepath: str):
        try:
            with open(filepath, "r") as f:
                self.__content = f.read()
        except Exception as e:
            print("bfvm: error: couldn't open file")
            exit(1)

        self.__curr = 0
        self.__size = len(self.__content)


    def next_token(self) -> Token:
        ch: str = self.__next_character()

        while not self.__is_bf_cmd(ch):
            ch = self.__next_character()
            if ch == '\0':
                return Token.END_OF_FILE

        match ch:
            case '+': return Token.PLUS
            case '-': return Token.MINUS
            case '<': return Token.ARROW_LEFT
            case '>': return Token.ARROW_RIGHT
            case '.': return Token.DOT
            case ',': return Token.COMMA
            case '[': return Token.BRACKET_LEFT
            case ']': return Token.BRACKET_RIGHT

        return Token.NONE

    def __next_character(self) -> str:        
        if self.__curr >= self.__size:
            return '\0'
        
        ch: str = self.__content[self.__curr]
        self.__curr += 1

        return ch

    def __is_bf_cmd(self, ch: str) -> bool:
        return ch == '+' or ch == '-' or ch == '<' or ch == '>' or ch == '.' or ch == ',' or ch == '[' or ch == ']'


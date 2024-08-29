from .bfc import Compiler, ByteCode, OpCode

def compile(filepath: str) -> list[ByteCode]:
    compiler = Compiler(filepath)
    return compiler.compile()

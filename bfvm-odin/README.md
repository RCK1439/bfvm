# BFVM - Odin

## Building

1. Create a `bin` directory
```bash
mkdir bin
```

2. Compiling
 * LINUX
 - Compiling without optimizations
```bash
odin build src/ -out:bin/bfvm
```
 - Compiling with optimizations
```bash
odin build src/ -out:bin/bfvm -o:aggressive
```

 * Windows
 - Compiling without optimizations
```bash
odin build src/ -out:bin/bfvm.exe
```
 - Compiling with optimizations
```bash
odin build src/ -out:bin/bfvm.exe -o:aggressive
```


## Running

Running is as easy as follows:

- Linux
```bash
./bin/bfvm <filepath>
```

- Windows
```bash
.\bin\bfvm.exe <filepath>
```
# BFVM: Brainfuck Virtual Machine

BFVM is a virtual machine built for the [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) programming language. The project started as a project that my university assigned to me as an assessment during my second year. The original idea was to write a Brainfuck interpreter in x86 assembly.

I wanted to make the programs run faster. I noticed that lots of operations get executed in "chains" (i.e. a line of '```+```' or a line of '```<```' operations). This could obviously be optimized into one operation. E.g. Add 10 to this byte, instead of ```++++++++++```. Or move the data pointer up by 5 instead of ```>>>>>```. So I built a tiny compiler that will first compile to a very simple "assembly" language of my own design to then be interpreted. This resulted in the creation of the Brainfuck Virtual Machine.

Now every time I need to learn a new language, I'll be adding it to this repository. I found it to be quite a nice learning exercise!

## Languages

The original VM was built in C, the next was built in C++ in an effort to learn modern C++. It is also interesting to note the performance differences between the languages.

## Performance

As mentioned before the performance differences were interesting to me. Note, the programs were measured very naively, but I had no intentions for serious metrics. The programs were measured on my laptop (10th gen Intel Core i5, 8GB RAM, Nvidia GeForce MX330 with 64-bit Ubuntu 22.04.4 LTS) simply by seeing how long it takes to run the ```mandelbrot.b``` program (you'll find this in the tests folder). I chose this program because it was the "most intense" one to run. Here are the results (in seconds):

|Language|Time (s) |
|:-------|:--------|
| C      | 6.153   |
| C++    | 6.295   |
| Rust   | 7.571   |

## Building

1. Ensure that you have the [clang](https://clang.llvm.org/) compiler installed on your system. 
2. In the C or C++ folder, run the following command: ```make release```.
This will build the executable with optimizations turned on to a ```bin``` directory.

## Installing

TODO

## Running

The virtual machine takes in only one argument, which is the filepath to a Brainfuck source file. Something like this: ```bfvm <source>```

### Linux

```./bfvm path/to/src/file.b```

Pretty straight forward, huh?

## Future plans

As of now I have no plans. For the future. Maybe I can improve the performance, but that day has yet to come. So this is what we have for now!

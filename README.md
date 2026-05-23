# YODA-CVM++ Compiler

YODA-CVM++ is a small experimental compiler for a new language called **YODA**. The language is currently C-like while the compiler pipeline is being built, but the plan is to make the syntax more YODA-themed over time.

At the moment, the compiler can lex, parse, generate x86-64 NASM assembly, assemble it, and link it into a Linux executable.

## Features

- Tokenizes `.yd` source files
- Builds an AST using a recursive descent parser
- Generates NASM x86-64 assembly
- Supports string `print` statements
- Supports integer `return`
- Supports integer variables
- Supports mathematical expressions with `+`, `-`, `*`, and `/`
- Supports parentheses in expressions

## Example

```yd
print("hi tai");
a=10;
b=2;

return a+b*2;
```

This should print:

```text
hi tai
```

and exit with code:

```text
14
```

Check the exit code with:

```bash
echo $?
```

## Requirements

This project currently targets Linux x86-64.

Install the required tools:

```bash
sudo apt update
sudo apt install g++ nasm binutils
```

Verify:

```bash
g++ --version
nasm -v
ld -v
```

## Build The Compiler

From the project root:

```bash
g++ pilot.cpp -o pilot
```

## Compile A YODA Program

Run:

```bash
./pilot example.yd
```

The compiler generates:

```text
example.yd.asm
example.o
example
```

Then run the compiled program:

```bash
./example
echo $?
```

## Manual Assembly Flow

If you want to assemble and link manually:

```bash
nasm -f elf64 example.yd.asm -o example.o
ld -o example example.o
./example
```

## Current Syntax

### Print Strings

```yd
print("Hello");
```

### Variables

```yd
a=10;
b=2;
```

### Return Expressions

```yd
return a+b*2;
```

### Parentheses

```yd
return (a+b)*2;
```

## Compiler Pipeline

```text
source .yd
   |
   v
Lexer
   |
   v
Parser
   |
   v
AST
   |
   v
Generator
   |
   v
NASM assembly
   |
   v
object file
   |
   v
executable
```

## Project Structure

```text
pilot.cpp              main compiler driver
example.yd             sample YODA source file
headers/lexer.hpp      lexer/tokenizer
headers/parser.hpp     parser and AST definitions
headers/generator.hpp  assembly generator
asmbl/test.asm         hand-written assembly experiments
```

## Notes And Limitations

- The language is still early and intentionally small.
- `print` currently focuses on strings.
- Integer expression printing needs extra runtime/codegen support because numbers must be converted to text before writing to stdout.
- Error messages are basic and will improve over time.
- The compiler currently emits Linux x86-64 assembly using direct syscalls.

## Roadmap

- Improve error messages with line and column info
- Support printing integer expressions
- Add better variable handling and scope rules
- Add comments in `.yd` files
- Add comparison operators and conditionals
- Add loops
- Move toward a more YODA-themed syntax


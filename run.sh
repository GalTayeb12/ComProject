#!/bin/bash

# Find this shell script's directory - DO NOT DELETE
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Compile the lex and yacc files - Change if needed
bison -d parser.y
flex scanner.l
gcc lex.yy.c parser.tab.c ast.c -o compiler -lfl
./compiler < test.txt > output.txt
asm:
	clang++ -std=c++17 -o assembler assembler.cpp parser.cpp code.cpp symboltable.cpp

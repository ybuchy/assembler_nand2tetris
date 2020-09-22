#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>

class SymbolTable {
        std::map<std::string, int> symbolTable{
                                               {"SP", 0},
                                               {"LCL", 1},
                                               {"ARG", 2},
                                               {"THIS", 3},
                                               {"THAT", 4},
                                               {"R0", 0},
                                               {"R1", 1},
                                               {"R2", 2},
                                               {"R3", 3},
                                               {"R4", 4},
                                               {"R5", 5},
                                               {"R6", 6},
                                               {"R7", 7},
                                               {"R8", 8},
                                               {"R9", 9},
                                               {"R10", 10},
                                               {"R11", 11},
                                               {"R12", 12},
                                               {"R13", 13},
                                               {"R14", 14},
                                               {"R15", 15},
                                               {"SCREEN", 16384},
                                               {"KBD", 24576}
        };
    public:
        void addEntry(const std::string symbol, const int address);
        bool contains(const std::string symbol);
        int getAddress(const std::string symbol);
};

#endif // SYMBOLTABLE_H

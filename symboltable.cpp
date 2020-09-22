#include "symboltable.hpp"

void SymbolTable::addEntry(const std::string symbol, const int address) {
    symbolTable[symbol] = address;
}

bool SymbolTable::contains(const std::string symbol) {
    return (symbolTable.count(symbol) != 0) ? true : false;
}

int SymbolTable::getAddress(const std::string symbol) {
    return symbolTable[symbol];
}

#include "parser.hpp"
#include "code.hpp"
#include "symboltable.hpp"
#include <fstream>
#include <bitset>

int main(int argc, char **argv) {
    const size_t ind = std::string(argv[1]).find('.');
    if (ind == std::string::npos) return 1;
    std::string filename = std::string(argv[1]).substr(0, ind);
    std::ofstream output;
    output.open(filename + ".hack");
    Parser sym_p = Parser(argv[1]);
    // Fill the symbol table
    int cur_ip = 0;
    int var_addr = 1024;
    SymbolTable symTable = SymbolTable();
    while(sym_p.hasMoreCommands()) {
        sym_p.advance();
        switch(sym_p.commandType()) {
            case A_Command:
                {
                    std::string sym = sym_p.symbol();
                    if (!std::isdigit(sym[0] && !symTable.contains(sym))) {
                        symTable.addEntry(sym, var_addr);
                        var_addr++;
                    }
                }
                break;
            case L_Command:
                {
                    std::string sym = sym_p.symbol();
                    symTable.addEntry(sym, cur_ip);
                    cur_ip--;
                }
                break;
            default:
                break;
        }
        cur_ip++;
    }
    Parser p = Parser(argv[1]);
    // Convert to bits + write to file
    Code c = Code();
    while(p.hasMoreCommands()) {
        p.advance();
        std::string line{};
        switch (p.commandType()) {
            case A_Command:
                {
                    std::string sym = p.symbol();
                    int addr = [&]() {
                                   if (!std::isdigit(sym[0]) &&
                                       symTable.contains(sym)) {
                                       return symTable.getAddress(sym);
                                   } else {
                                       return std::stoi(sym);
                                   }
                               }();
                    line = "0" + std::bitset<15>(addr).to_string();
                }
                break;
            case C_Command:
                line = "111" +
                    c.comp(p.comp()) +
                    c.dest(p.dest()) +
                    c.jump(p.jump());
                break;
            default:
                break;
        };
        if (p.commandType() != L_Command) output << line << '\n';
    }
    return 0;
}

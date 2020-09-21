#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <bitset>
#include <iostream>

enum Command {A_Command, C_Command, L_Command};

class Parser {
        std::vector<std::string> lines;
        std::string cur_line;
    public:
        Parser(std::string f);
        bool hasMoreCommands(void);
        void advance(void);
        Command commandType(void);
        std::string symbol(void);
        std::string dest(void);
        std::string comp(void);
        std::string jump(void);
};

Parser::Parser(std::string f) {
    std::ifstream file;
    file.open(f);
    std::string line;
    while (std::getline(file, line)) { 
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
	const size_t ind = line.find("//");
        if (ind != std::string::npos) line = line.substr(0, ind);
        if (line != "") lines.push_back(line);
    }
    std::reverse(lines.begin(), lines.end());
}

bool Parser::hasMoreCommands(void) {
    return (lines.size() > 0) ? true : false;
}

void Parser::advance(void) {
    cur_line = lines.back();
    lines.pop_back();
}

Command Parser::commandType(void) {
    if (cur_line[0] == '@') return A_Command;
    if (cur_line[0] == '(') return L_Command;
    else return C_Command;
}

std::string Parser::symbol(void) {
    if (cur_line[0] == '@') return cur_line.substr(1);
    if (cur_line[0] == '(') return cur_line.substr(1, cur_line.size()-2);
    else return "";
}

std::string Parser::dest(void) {
    const size_t ind = cur_line.find('=');
    return (ind != std::string::npos) ? cur_line.substr(0, ind) : "";
}

std::string Parser::comp(void) {
    const size_t ind = cur_line.find('=');
    const size_t ind2 = cur_line.find(';');
    
    if (ind != std::string::npos) {
        if (ind2 != std::string::npos) {
            return cur_line.substr(ind+1, ind2);
        } else {
            return cur_line.substr(ind+1);
        }
    } else {
        return cur_line.substr(0, ind2);
    }
}

std::string Parser::jump(void) {
    const size_t ind = cur_line.find(';');
    if (ind != std::string::npos) return cur_line.substr(ind+1);
    else return "";
}

class Code {
    public:
        std::string dest(const std::string mnemonic);
        std::string comp(const std::string mnemonic);
        std::string jump(const std::string mnemonic);
} code;

std::string Code::dest(const std::string mnemonic) {
    const std::array<std::string, 3> s{"A", "D", "M"};
    std::map<std::string, std::string> bits;

    auto get_bit = [&](const std::string s){bits[s] = (mnemonic.find(s) == std::string::npos) ? "0" : "1";};

    std::for_each(std::begin(s), std::end(s), get_bit);

    return bits["A"] + bits["D"] + bits["M"];
}

std::string Code::comp(const std::string mnemonic) {
    std::map<std::string, std::string> m{
					 {"0", "0101010"},
					 {"1", "0111111"},
					 {"-1", "0111010"},
					 {"D", "0001100"},
					 {"A", "0110000"},
					 {"!D", "0001101"},
					 {"!A", "0110001"},
					 {"-D", "0001111"},
					 {"-A", "0110011"},
					 {"D+1", "0011111"},
					 {"A+1", "0110111"},
					 {"D-1", "0001110"},
					 {"A-1", "0110010"},
					 {"D+A", "0000010"},
					 {"D-A", "0010011"},
					 {"A-D", "0000111"},
					 {"D&A", "0000000"},
					 {"D|A", "0010101"},
					 {"M", "1110000"},
					 {"!M", "1110001"},
					 {"-M", "1110011"},
					 {"M+1", "1110111"},
					 {"M-1", "1110010"},
					 {"D+M", "1000010"},
					 {"D-M", "1010011"},
					 {"M-D", "1000111"},
					 {"D&M", "1000000"},
					 {"D|M", "1010101"}
    };

    return m[mnemonic];
}

std::string Code::jump(const std::string mnemonic) {
    std::map<std::string, std::string> m{
					 {"", "000"},
					 {"JGT", "001"},
					 {"JEQ", "010"},
					 {"JGE", "011"},
					 {"JLT", "100"},
					 {"JNE", "101"},
					 {"JLE", "110"},
					 {"JMP", "111"}
    };

    return m[mnemonic];
}

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

void SymbolTable::addEntry(const std::string symbol, const int address) {
    symbolTable[symbol] = address;
}

bool SymbolTable::contains(const std::string symbol) {
    return (symbolTable.count(symbol) != 0) ? true : false;
}

int SymbolTable::getAddress(const std::string symbol) {
    return symbolTable[symbol];
}

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
		std::string sym = sym_p.symbol();
                symTable.addEntry(sym, cur_ip);
                cur_ip--;
                break;
        }
        cur_ip++;
    }
    Parser p = Parser(argv[1]);
    // Convert to bits + write to file
    while(p.hasMoreCommands()) {
        p.advance();
	std::string line{};
        switch (p.commandType()) {
            case A_Command:
		{
		    std::string sym = p.symbol();
		    int addr = [&]() {
		    		   if (!std::isdigit(sym[0]) && symTable.contains(sym)) {
		    		       return symTable.getAddress(sym);
		    		   } else {
		    		       return std::stoi(sym);
		    		   }
		    	       }();
		    line = "0" + std::bitset<15>(addr).to_string();
		}
                break;

            case C_Command:
		line = "111" + code.comp(p.comp()) + code.dest(p.dest()) + code.jump(p.jump());
                break;
        };
        if (p.commandType() != L_Command) output << line << '\n';
    }
    return 0;
}

#include "parser.hpp"
#include <fstream>
#include <algorithm>

Parser::Parser(std::string f) {
    std::ifstream file;
    file.open(f);
    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), isspace),
                   line.end());
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

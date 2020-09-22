#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

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

#endif // PARSER_H

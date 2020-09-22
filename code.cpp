#include "code.hpp"
#include <array>
#include <map>
#include <algorithm>

std::string Code::dest(const std::string mnemonic) {
    const std::array<std::string, 3> s{"A", "D", "M"};
    std::map<std::string, std::string> bits;

    auto get_bit = [&](const std::string s) {bits[s] =
            (mnemonic.find(s) == std::string::npos) ? "0" : "1";};

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

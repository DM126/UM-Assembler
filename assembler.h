#include <cstdint>
#include <stack>
#include <string>

struct Token
{
    bool isImmediate; //True if immediate, false if register
    uint32_t value; //immediate value or register number
};

class Assembler
{
public:
    Assembler(const std::string& filename);
private:
    void readInput(std::ifstream& infile);
    void parseLine(const std::string& line);
    
    //After encountering 'r' or 'R', determine what register.
    //i should be the index of the register number in the line.
    //Throws invalid_argument if not a valid register.
    void parseRegister(const string& line, unsigned int i);
    
    //Parses an unsigned integer immediate value after encountering a digit.
    //i should be the index of the first encountered digit.
    //Throws invalid_argument if not a valid unsigned integer.
    void parseImmediate(const string& line, unsigned int i);
    
    std::stack<Token> tokenStack;
};

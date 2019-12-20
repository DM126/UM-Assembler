#include <cstdint>
#include <fstream>
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
    void parseRegister(const std::string& line, unsigned int i);
    
    //Parses an unsigned integer immediate value after encountering a digit.
    //start should be the index of the first encountered digit.
    //Throws invalid_argument if not a valid unsigned integer.
    //Returns the index within the line after the immediate, the value itself
    //is placed on the stack.
    unsigned int parseImmediate(const std::string& line, const unsigned int start);
    
    std::stack<Token> tokenStack;
};

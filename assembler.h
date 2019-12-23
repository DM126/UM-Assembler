#include <cstdint>
#include <fstream>
#include <stack>
#include <string>

//Operation mnemonics
const static int NUM_OPERATIONS = 14;
const static int OPCODE_BITS = 4; //bits required to identify the 14 operations
const static std::string operations[] = {"CMOV", "INDEX", "AMEND", "ADD", "MUL", 
                                        "DIV", "NAND", "HALT", "ALLOC", "ABAND", 
                                        "OUT", "IN", "LOAD", "ORTH"};
                                        
//TODO make operation struct, then use just one array

//The number of registers each operation uses. Compared with the size of the 
//token stack when creating the binary instruction code.
const static int REGISTER_BITS = 3; //bits required to identify the 8 registers
const static int registers[] = {3, 3, 3, 3, 3,
                                3, 3, 0, 3, 1,
                                1, 1, 2, 2};

struct Token
{
    bool isImmediate; //True if immediate, false if register
    uint32_t value; //immediate value or register number
};

class Assembler
{
public:
    Assembler(const std::string& filename);

    //Symbol to begin a comment
    const char COMMENT = '#';
    
    const int BITS_PER_WORD = 32;

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
    //Returns the index of the last char of the value within the line, the value
    //itself is placed on the stack.
    unsigned int parseImmediate(const std::string& line, const unsigned int start);
    
    //Parses an operation, then determines how to create the opcode.
    //start should be the index of the first encountered char of the operation.
    //Throws invalid_argument if the operation does not exist.
    //Returns the index of the last char of the operation within the line.
    unsigned int parseOperation(const std::string& line, unsigned int start);
    
    //Writes the binary instruction code to the file
    void createInstruction(const std::string& op);
    
    //Removes and returns the token on top of the token stack
    Token popStack();
    
    std::stack<Token> tokenStack;
    
    //Binary output file
    std::ofstream outfile;
};

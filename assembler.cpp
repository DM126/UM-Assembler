#include "assembler.h"

using namespace std;

Assembler::Assembler(const string& fileName)
{
    outfile = ofstream("um.o", ios::binary);
    
    ifstream infile(fileName);
    if (infile.fail())
    {
        throw runtime_error("Error: Could not find file: " + fileName);
    }
    else
    {
        readInput(infile);
    }
    
    infile.close();
    outfile.close();
}

void Assembler::readInput(ifstream& infile)
{
    string line;
    
    while (!infile.eof())
    {
        std::getline(infile, line);
        parseLine(line);
    }
}

//TODO Throw exception if no operation found
void Assembler::parseLine(const string& line)
{
    bool operationFound = false; //True once the operation is found. Anything after the operation will cause an error.
    for (unsigned int i = 0; i < line.length(); i++)
    {
        if (!isspace(line[i]))
        {
            if (line[i] == COMMENT) //Comment
            {
                break;
            }
            else if (operationFound) //If operation already found, throw an exception.
            {
                throw invalid_argument("Error: Encountered a symbol after operation.");
            }
            else if (isdigit(line[i])) //Immediate value
            {
                i = parseImmediate(line, i);
            }
            else if (line[i] == 'r' || line[i] == 'R') //Register
            {
                parseRegister(line, i+1);
                i++; //Skip number
            }
            else
            {
                i = parseOperation(line, i);
                operationFound = true;
            }
        }
    }
}

void Assembler::parseRegister(const string& line, unsigned int i)
{
    if (i < line.length() && line[i] >= '0' && line[i] <= '7')
    {
        Token t = {
            .isImmediate = false,
            .value = static_cast<uint32_t>(line[i] - '0') //Convert ascii to int
        };
        tokenStack.push(t);
    }
    else
    {
        throw invalid_argument("Invalid register.");
    }
}

unsigned int Assembler::parseImmediate(const string& line, const unsigned int start)
{
    unsigned int length = 1; //Number of chars in int
    for (unsigned int i = start + 1; i < line.length(); i++)
    {
        char c = line[i];
        if (isspace(c) || c == COMMENT)
        {
            break;
        }
        else if (!isdigit(c))
        {
            throw invalid_argument("Invalid immediate value");
        }
        
        length++;
    }
    
    string intString = line.substr(start, length);
    Token immed = {
        .isImmediate = true,
        .value = static_cast<uint32_t>(stoul(intString))
    };
    tokenStack.push(immed);
    
    return start + length - 1;
}

unsigned int Assembler::parseOperation(const string& line , unsigned int start)
{
    //TODO This is almost an exact duplicate of parseImmediate.
    //Could pass a function pointer of the stl function to a generic function...
    
    unsigned int length = 1; //Number of chars in int
    for (unsigned int i = start + 1; i < line.length(); i++)
    {
        char c = line[i];
        if (isspace(c) || c == COMMENT)
        {
            break;
        }
        else if (!isalpha(c))
        {
            throw invalid_argument("Invalid operation value");
        }
        
        length++;
    }
    
    string operation = line.substr(start, length);
    createInstruction(operation);
    
    return start + length - 1;
}

void Assembler::createInstruction(const string& op)
{
    uint32_t instruction = 0;
    
    for (int i = 0; i < NUM_OPERATIONS; i++)
    {
        if (op == operations[i])
        {
            if (tokenStack.size() != registers[i])
            {
                //TODO Be more descriptive? Refactor?
                throw invalid_argument("Incorrect number of arguments for " + operations[i]);
            }
            
            instruction = (i << BITS_PER_WORD - OPCODE_BITS); //opcode
            
            if (operations[i] == "ORTH") //Special immediate load instruction
            {
                Token regist = popStack();
                Token immed = popStack();
                if (!regist.isImmediate && immed.isImmediate)
                {
                    instruction += (regist.value << BITS_PER_WORD - OPCODE_BITS - REGISTER_BITS);
                    //TODO Error check if immed.value >= 2^25?
                    instruction += (immed.value & 0b00000001111111111111111111111111); //Must fit into 25 bits
                }
                else
                {
                    throw invalid_argument("ORTH Must have an immediate followed by a register.");
                }
            }
            else
            {
                //Filler bits not used by opcode or registers.
                //instruction += 0b00001111111111111111111000000000;
                
                while (!tokenStack.empty())
                {
                    Token t = popStack();
                    
                    if (!t.isImmediate)
                    {
                        instruction += (t.value << tokenStack.size() * REGISTER_BITS);
                    }
                    else
                    {
                        throw invalid_argument("Argument must be a register.");
                    }
                }
            }
            
            break;
        }
    }
    
    outfile.write((char*)&instruction, 4); //4 bytes per word
}

Token Assembler::popStack()
{
    Token t = tokenStack.top();
    tokenStack.pop();
    
    return t;
}

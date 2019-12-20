#include "assembler.h"

using namespace std;

Assembler::Assembler(const string& fileName)
{
    ifstream infile(fileName);
    if (infile.fail())
	{
		throw runtime_error("Error: Could not find file: " + fileName);
	}
	else
	{
		readInput(infile);
	}
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

void Assembler::parseLine(const string& line)
{
    bool operationFound = false; //True once the operation is found. Anything after the operation will cause an error.
    for (unsigned int i = 0; i < line.length(); i++)
    {
        if (!isspace(line[i]))
        {
            if (line[i] == '#') //Comment
            {
                break;
            }
            else if (isdigit(line[i])) //Immediate value
            {
                parseImmediate(line, i);
                i++ //Skip number
            }
            else if (line[i] == 'r' || line[i] == 'R') //Register
            {
                i = parseRegister(line, i+1);
            }
            else
            {
                parseOperation(line, i);
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
        if (isspace(c) || c == '#')
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

void Assembler::parseOperation(const string& line , unsigned int start)
{
    //TODO This is almost an exact duplicate of parseImmediate.
    //Could pass a function pointer of the stl function to a generic function...
    
    unsigned int length = 1; //Number of chars in int
    for (unsigned int i = start + 1; i < line.length(); i++)
    {
        char c = line[i];
        if (isspace(c) || c == '#')
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

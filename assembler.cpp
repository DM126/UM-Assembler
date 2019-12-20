#include "assembler.h"
#include <fstream>

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
            }
            else if (line[i] == 'r' || line[i] == 'R') //Register
            {
                parseRegister(line, i+1);
            }
        }
    }
}

void Assembler::parseRegister(const string& line, int i)
{
    if (i < line.length() && line[i] >= '0' && line[i] <= '7')
    {
        Token t;
        t.isImmediate = false;
        t.value = line[i] - '0'; //Convert ascii to int
        tokenStack.push(t)
    }
    else
    {
        throw invalid_argument("Invalid register.");
    }
}
